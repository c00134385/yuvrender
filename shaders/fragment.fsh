varying vec2 v_texCoord;

uniform sampler2D tex_y;
uniform sampler2D tex_u;
uniform sampler2D tex_v;
uniform int pixFmt;
void main(void)
{
    vec3 yuv;
    vec3 rgb;
    if (pixFmt == 0) {
        //yuv420p
        yuv.x = texture2D(tex_y, v_texCoord).r;
        yuv.y = texture2D(tex_u, v_texCoord).r - 0.5;
        yuv.z = texture2D(tex_v, v_texCoord).r - 0.5;
//        rgb = mat3( 1,       1,         1,
//                    0,       -0.39465,  2.03211,
//                    1.13983, -0.58060,  0) * yuv;
        rgb = mat3( 1.0,       1.0,         1.0,
                        0.0,       -0.3455,  1.779,
                        1.4075, -0.7169,  0.0) * yuv;
    } else if (pixFmt == 1) {
        // display grey bitmap
        rgb.x = texture2D(tex_y, v_texCoord).r;
        rgb.y = texture2D(tex_y, v_texCoord).r;
        rgb.z = texture2D(tex_y, v_texCoord).r;
    } else if(pixFmt == 2) {
        float grey = texture2D(tex_y, v_texCoord).r;
        if(grey > 0.8) {
            rgb.x = 1.0;
            rgb.y = 0.0;
            rgb.z = 0.0;
        } else {
            rgb.x = grey;
            rgb.y = grey;
            rgb.z = grey;
        }
    } else {
        //YUV444P
        yuv.x = texture2D(tex_y, v_texCoord).r;
        yuv.y = texture2D(tex_u, v_texCoord).r - 0.5;
        yuv.z = texture2D(tex_v, v_texCoord).r - 0.5;

        rgb.x = clamp( yuv.x + 1.402 *yuv.z, 0.0, 1.0);
        rgb.y = clamp( yuv.x - 0.34414 * yuv.y - 0.71414 * yuv.z, 0.0, 1.0);
        rgb.z = clamp( yuv.x + 1.772 * yuv.y, 0.0, 1.0);
    }
    gl_FragColor = vec4(rgb, 1.0);
//    gl_FragColor = vec4(1.0, 1.0, 0, 1.0);
}

