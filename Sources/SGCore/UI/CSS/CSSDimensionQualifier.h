//
// Created by stuka on 21.01.2025.
//

#ifndef SUNGEARENGINE_CSSDIMENSIONQUALIFIER_H
#define SUNGEARENGINE_CSSDIMENSIONQUALIFIER_H

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
        DQ_PERCENT
    };
}

#endif //SUNGEARENGINE_CSSDIMENSIONQUALIFIER_H
