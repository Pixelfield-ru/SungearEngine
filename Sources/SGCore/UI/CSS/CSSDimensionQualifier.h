//
// Created by stuka on 21.01.2025.
//

#ifndef SUNGEARENGINE_CSSDIMENSIONQUALIFIER_H
#define SUNGEARENGINE_CSSDIMENSIONQUALIFIER_H

#include <string>

namespace SGCore::UI
{
    enum struct CSSDimensionQualifier
    {
        // ================================================================ absolute length
        DQ_PX,      // pixels
        DQ_CM,      // centimeters
        DQ_MM,      // millimeters
        DQ_IN,      // inches
        DQ_PT,      // points
        DQ_PC,      // picas (1 pica = 12 points)
        DQ_Q,       // quarter of a millimeter
        // ================================================================ angle
        DQ_DEG,     // degrees
        DQ_RAD,     // radians
        DQ_GRAD,    // gradians
        DQ_TURN,    // 1turn = 360deg
        // ================================================================ time
        DQ_MS,      // milliseconds
        DQ_S,       // seconds
        // ================================================================ frequency
        DQ_HZ,      // hertz
        DQ_KHZ,     // kilohertz,
        // ================================================================ relative units
        DQ_EM,      // size relative to parent element font-size
        DQ_EX,      // relative to size of character 'x' in current font
        DQ_CH,      // relative to size of character '0' in current font
        DQ_REM,     // size relative to root element font-size
        DQ_VW,      // viewport (window) width
        DQ_VH,      // viewport (window) height
        DQ_VMIN,    // minimum of VW and VH
        DQ_VMAX,    // maximum of VW and VH
        // ================================================================ resolution units
        DQ_DPI,     // dots per inch
        DQ_DPCM,    // dots per centimeter
        DQ_DPPX,    // dots per pixel
        // ================================================================
        DQ_PERCENT,

        // ================================================================ special values
        DQ_NUMBER, // equals to DQ_PX
        DQ_ANY // for qualifiers support. this value means that current context supports all qualifiers
    };

    static CSSDimensionQualifier getDimensionQualifierFromString(const std::string& dimensionStr) noexcept
    {
        // ================================================================ absolute length
        if(dimensionStr.ends_with("px"))
        {
            return CSSDimensionQualifier::DQ_PX;
        }
        else if(dimensionStr.ends_with("cm"))
        {
            return CSSDimensionQualifier::DQ_CM;
        }
        else if(dimensionStr.ends_with("mm"))
        {
            return CSSDimensionQualifier::DQ_MM;
        }
        else if(dimensionStr.ends_with("in"))
        {
            return CSSDimensionQualifier::DQ_IN;
        }
        else if(dimensionStr.ends_with("pt"))
        {
            return CSSDimensionQualifier::DQ_PT;
        }
        else if(dimensionStr.ends_with("pc"))
        {
            return CSSDimensionQualifier::DQ_PC;
        }
        else if(dimensionStr.ends_with("q"))
        {
            return CSSDimensionQualifier::DQ_Q;
        }
            // ================================================================ angle
        else if(dimensionStr.ends_with("deg"))
        {
            return CSSDimensionQualifier::DQ_DEG;
        }
        else if(dimensionStr.ends_with("rad"))
        {
            return CSSDimensionQualifier::DQ_RAD;
        }
        else if(dimensionStr.ends_with("grad"))
        {
            return CSSDimensionQualifier::DQ_GRAD;
        }
        else if(dimensionStr.ends_with("turn"))
        {
            return CSSDimensionQualifier::DQ_TURN;
        }
            // ================================================================ time
        else if(dimensionStr.ends_with("ms"))
        {
            return CSSDimensionQualifier::DQ_MS;
        }
        else if(dimensionStr.ends_with("s"))
        {
            return CSSDimensionQualifier::DQ_S;
        }
            // ================================================================ frequency
        else if(dimensionStr.ends_with("hz"))
        {
            return CSSDimensionQualifier::DQ_HZ;
        }
        else if(dimensionStr.ends_with("khz"))
        {
            return CSSDimensionQualifier::DQ_KHZ;
        }
            // ================================================================ relative units
        else if(dimensionStr.ends_with("em"))
        {
            return CSSDimensionQualifier::DQ_EM;
        }
        else if(dimensionStr.ends_with("ex"))
        {
            return CSSDimensionQualifier::DQ_EX;
        }
        else if(dimensionStr.ends_with("ch"))
        {
            return CSSDimensionQualifier::DQ_CH;
        }
        else if(dimensionStr.ends_with("rem"))
        {
            return CSSDimensionQualifier::DQ_REM;
        }
        else if(dimensionStr.ends_with("vw"))
        {
            return CSSDimensionQualifier::DQ_VW;
        }
        else if(dimensionStr.ends_with("vh"))
        {
            return CSSDimensionQualifier::DQ_VH;
        }
        else if(dimensionStr.ends_with("vmin"))
        {
            return CSSDimensionQualifier::DQ_VMIN;
        }
        else if(dimensionStr.ends_with("vmax"))
        {
            return CSSDimensionQualifier::DQ_VMAX;
        }
            // ================================================================ resolution units
        else if(dimensionStr.ends_with("dpi"))
        {
            return CSSDimensionQualifier::DQ_DPI;
        }
        else if(dimensionStr.ends_with("dpcm"))
        {
            return CSSDimensionQualifier::DQ_DPCM;
        }
        else if(dimensionStr.ends_with("dppx"))
        {
            return CSSDimensionQualifier::DQ_DPPX;
        }
        else if(dimensionStr.ends_with("%"))
        {
            return CSSDimensionQualifier::DQ_PERCENT;
        }

        return CSSDimensionQualifier::DQ_NUMBER;
    }

    static std::string dimensionQualifierToString(const CSSDimensionQualifier& qualifier) noexcept
    {
        switch(qualifier)
        {
            case CSSDimensionQualifier::DQ_PX: return "px";
            case CSSDimensionQualifier::DQ_CM: return "cm";
            case CSSDimensionQualifier::DQ_MM: return "mm";
            case CSSDimensionQualifier::DQ_IN: return "in";
            case CSSDimensionQualifier::DQ_PT: return "pt";
            case CSSDimensionQualifier::DQ_PC: return "pc";
            case CSSDimensionQualifier::DQ_Q: return "q";
            case CSSDimensionQualifier::DQ_DEG: return "deg";
            case CSSDimensionQualifier::DQ_RAD: return "rad";
            case CSSDimensionQualifier::DQ_GRAD: return "grad";
            case CSSDimensionQualifier::DQ_TURN: return "turn";
            case CSSDimensionQualifier::DQ_MS: return "ms";
            case CSSDimensionQualifier::DQ_S: return "s";
            case CSSDimensionQualifier::DQ_HZ: return "hz";
            case CSSDimensionQualifier::DQ_KHZ: return "khz";
            case CSSDimensionQualifier::DQ_EM: return "em";
            case CSSDimensionQualifier::DQ_EX: return "ex";
            case CSSDimensionQualifier::DQ_CH: return "ch";
            case CSSDimensionQualifier::DQ_REM: return "rem";
            case CSSDimensionQualifier::DQ_VW: return "vw";
            case CSSDimensionQualifier::DQ_VH: return "vh";
            case CSSDimensionQualifier::DQ_VMIN: return "vmin";
            case CSSDimensionQualifier::DQ_VMAX: return "vmax";
            case CSSDimensionQualifier::DQ_DPI: return "dpi";
            case CSSDimensionQualifier::DQ_DPCM: return "dpcm";
            case CSSDimensionQualifier::DQ_DPPX: return "dppx";
            case CSSDimensionQualifier::DQ_PERCENT: return "%";
            case CSSDimensionQualifier::DQ_NUMBER: return "number";
            case CSSDimensionQualifier::DQ_ANY: return "any";
        }

        return "";
    }
}

#endif //SUNGEARENGINE_CSSDIMENSIONQUALIFIER_H
