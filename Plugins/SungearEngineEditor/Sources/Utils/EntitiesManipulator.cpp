//
// Created by stuka on 23.12.2024.
//
#include "EntitiesManipulator.h"

void SGE::EntitiesManipulator::getDeltaBetweenMatrices(const glm::mat4& originalMatrix, const glm::mat4& updatedMatrix,
                                                       glm::vec3& deltaTranslation, glm::quat& deltaRotation,
                                                       glm::vec3& deltaScale) noexcept
{
    glm::vec3 originalTranslation, originalScale, skew;
    glm::quat originalRotation;
    glm::vec4 perspective;
    glm::decompose(originalMatrix, originalScale, originalRotation, originalTranslation, skew, perspective);

    glm::vec3 updatedTranslation, updatedScale;
    glm::quat updatedRotation;
    glm::decompose(updatedMatrix, updatedScale, updatedRotation, updatedTranslation, skew, perspective);

    deltaTranslation = updatedTranslation - originalTranslation;
    deltaScale = updatedScale / originalScale;

    deltaRotation = updatedRotation * glm::inverse(originalRotation);
    // deltaRotation = glm::inverse(updatedRotation) * originalRotation;
}

void SGE::EntitiesManipulator::rotateAroundWorldQuaternion(glm::quat& localRotation, const glm::quat& worldRotation) noexcept
{
    // Инвертируем локальный поворот, чтобы преобразовать мировое вращение в локальные координаты
    glm::quat inverseRotation = glm::inverse(localRotation);

    // Преобразуем мировой кватернион вращения в локальную систему координат
    glm::quat localRotationDelta = inverseRotation * worldRotation * localRotation;

    // Обновляем локальный кватернион вращения
    localRotation = localRotation * localRotationDelta;
}

/*void SGE::EntitiesManipulator::rotateAroundWorldAxis(glm::mat4& localMatrix, const glm::mat4& parentMatrix,
                                                     const glm::vec3& worldAxis, float angle) noexcept
{
// Вычисление мировой матрицы
    glm::mat4 worldMatrix = parentMatrix * localMatrix;

    // Матрица вращения вокруг мировой оси
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), worldAxis);

    // Применяем вращение в мировом пространстве
    glm::mat4 worldMatrixRotated = rotationMatrix * worldMatrix;

    // Возвращаем матрицу в локальное пространство
    glm::mat4 parentInverse = glm::inverse(parentMatrix);
    localMatrix = parentInverse * worldMatrixRotated;
}*/

