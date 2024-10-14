#pragma once

#include <glm/gtx/matrix_decompose.hpp>

/*
* MisterElect (MisterChoose)
* 04.10.2024
*/

namespace glm {
	namespace extension {
		template<typename T, qualifier Q>
		// Rewritten foo without for's
		// Specially for MSVC (i've got bad optimizations on fors, even if they can be precalc)
		GLM_FUNC_QUALIFIER bool decompose(mat<4, 4, T, Q> const& ModelMatrix, vec<3, T, Q>& Scale, qua<T, Q>& Orientation, vec<3, T, Q>& Translation, vec<3, T, Q>& Skew, vec<4, T, Q>& Perspective)
		{
			mat<4, 4, T, Q> LocalMatrix(ModelMatrix);

			const auto& lm3_3 = LocalMatrix[3][3];

			// Normalize the matrix.
			if (epsilonEqual(lm3_3, static_cast<T>(0), epsilon<T>()))
				return false;

			LocalMatrix[0][0] /= lm3_3;
			LocalMatrix[0][1] /= lm3_3;
			LocalMatrix[0][2] /= lm3_3;
			LocalMatrix[0][3] /= lm3_3;

			LocalMatrix[1][0] /= lm3_3;
			LocalMatrix[1][1] /= lm3_3;
			LocalMatrix[1][2] /= lm3_3;
			LocalMatrix[1][3] /= lm3_3;

			LocalMatrix[2][0] /= lm3_3;
			LocalMatrix[2][1] /= lm3_3;
			LocalMatrix[2][2] /= lm3_3;
			LocalMatrix[2][3] /= lm3_3;

			LocalMatrix[3][0] /= lm3_3;
			LocalMatrix[3][1] /= lm3_3;
			LocalMatrix[3][2] /= lm3_3;
			LocalMatrix[3][3] /= lm3_3;

			// perspectiveMatrix is used to solve for perspective, but it also provides
			// an easy way to test for singularity of the upper 3x3 component.
			mat<4, 4, T, Q> PerspectiveMatrix(LocalMatrix);

			PerspectiveMatrix[0][3] = static_cast<T>(0);
			PerspectiveMatrix[1][3] = static_cast<T>(0);
			PerspectiveMatrix[2][3] = static_cast<T>(0);
			PerspectiveMatrix[3][3] = static_cast<T>(1);

			/// TODO: Fixme!
			if (epsilonEqual(determinant(PerspectiveMatrix), static_cast<T>(0), epsilon<T>()))
				return false;

			// First, isolate perspective.  This is the messiest.
			if (
				epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
				epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
				epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
			{
				// rightHandSide is the right hand side of the equation.
				vec<4, T, Q> RightHandSide;
				RightHandSide[0] = LocalMatrix[0][3];
				RightHandSide[1] = LocalMatrix[1][3];
				RightHandSide[2] = LocalMatrix[2][3];
				RightHandSide[3] = LocalMatrix[3][3];

				// Solve the equation by inverting PerspectiveMatrix and multiplying
				// rightHandSide by the inverse.  (This is the easiest way, not
				// necessarily the best.)
				mat<4, 4, T, Q> InversePerspectiveMatrix = glm::inverse(PerspectiveMatrix);//   inverse(PerspectiveMatrix, inversePerspectiveMatrix);
				mat<4, 4, T, Q> TransposedInversePerspectiveMatrix = glm::transpose(InversePerspectiveMatrix);//   transposeMatrix4(inversePerspectiveMatrix, transposedInversePerspectiveMatrix);

				Perspective = TransposedInversePerspectiveMatrix * RightHandSide;
				//  v4MulPointByMatrix(rightHandSide, transposedInversePerspectiveMatrix, perspectivePoint);

				// Clear the perspective partition
				LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
				LocalMatrix[3][3] = static_cast<T>(1);
			}
			else
			{
				// No perspective.
				Perspective = vec<4, T, Q>(0, 0, 0, 1);
			}

			// Next take care of translation (easy).
			Translation = vec<3, T, Q>(LocalMatrix[3]);
			LocalMatrix[3] = vec<4, T, Q>(0, 0, 0, LocalMatrix[3].w);

			vec<3, T, Q> Row[3], Pdum3;

			// Now get scale and shear.

			Row[0][0] = LocalMatrix[0][0];
			Row[0][1] = LocalMatrix[0][1];
			Row[0][2] = LocalMatrix[0][2];

			Row[1][0] = LocalMatrix[1][0];
			Row[1][1] = LocalMatrix[1][1];
			Row[1][2] = LocalMatrix[1][2];

			Row[2][0] = LocalMatrix[2][0];
			Row[2][1] = LocalMatrix[2][1];
			Row[2][2] = LocalMatrix[2][2];

			// Compute X scale factor and normalize first row.
			Scale.x = length(Row[0]);// v3Length(Row[0]);

			Row[0] = detail::scale(Row[0], static_cast<T>(1));

			// Compute XY shear factor and make 2nd row orthogonal to 1st.
			Skew.z = dot(Row[0], Row[1]);
			Row[1] = detail::combine(Row[1], Row[0], static_cast<T>(1), -Skew.z);

			// Now, compute Y scale and normalize 2nd row.
			Scale.y = length(Row[1]);
			Row[1] = detail::scale(Row[1], static_cast<T>(1));
			Skew.z /= Scale.y;

			// Compute XZ and YZ shears, orthogonalize 3rd row.
			Skew.y = glm::dot(Row[0], Row[2]);
			Row[2] = detail::combine(Row[2], Row[0], static_cast<T>(1), -Skew.y);
			Skew.x = glm::dot(Row[1], Row[2]);
			Row[2] = detail::combine(Row[2], Row[1], static_cast<T>(1), -Skew.x);

			// Next, get Z scale and normalize 3rd row.
			Scale.z = length(Row[2]);
			Row[2] = detail::scale(Row[2], static_cast<T>(1));
			Skew.y /= Scale.z;
			Skew.x /= Scale.z;

			// At this point, the matrix (in rows[]) is orthonormal.
			// Check for a coordinate system flip.  If the determinant
			// is -1, then negate the matrix and the scaling factors.
			Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
			if (dot(Row[0], Pdum3) < 0)
			{
				Scale[0] *= static_cast<T>(-1);
				Row[0] *= static_cast<T>(-1);

				Scale[1] *= static_cast<T>(-1);
				Row[1] *= static_cast<T>(-1);

				Scale[2] *= static_cast<T>(-1);
				Row[2] *= static_cast<T>(-1);
			}

			int i, j, k = 0;
			T root, trace = Row[0].x + Row[1].y + Row[2].z;
			if (trace > static_cast<T>(0))
			{
				root = sqrt(trace + static_cast<T>(1.0));
				Orientation.w = static_cast<T>(0.5) * root;
				root = static_cast<T>(0.5) / root;
				Orientation.x = root * (Row[1].z - Row[2].y);
				Orientation.y = root * (Row[2].x - Row[0].z);
				Orientation.z = root * (Row[0].y - Row[1].x);
			} // End if > 0
			else
			{
				static int Next[3] = { 1, 2, 0 };
				i = 0;
				if (Row[1].y > Row[0].x) i = 1;
				if (Row[2].z > Row[i][i]) i = 2;
				j = Next[i];
				k = Next[j];

#           ifdef GLM_FORCE_QUAT_DATA_WXYZ
				int off = 1;
#           else
				int off = 0;
#           endif

				root = sqrt(Row[i][i] - Row[j][j] - Row[k][k] + static_cast<T>(1.0));

				Orientation[i + off] = static_cast<T>(0.5) * root;
				root = static_cast<T>(0.5) / root;
				Orientation[j + off] = root * (Row[i][j] + Row[j][i]);
				Orientation[k + off] = root * (Row[i][k] + Row[k][i]);
				Orientation.w = root * (Row[j][k] - Row[k][j]);
			} // End if <= 0

			return true;
		}
	}
}