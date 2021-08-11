import QtQuick 2.4

ShaderEffect {
	property real cutOffHeight: 0.0
	property real alphaOffset: 0.0
	vertexShader: "
        uniform highp mat4 qt_Matrix;
        uniform highp float alphaOffset;
        attribute highp vec4 qt_Vertex;
        attribute highp vec2 qt_MultiTexCoord0;
        varying highp vec2 texc1;
        varying highp vec2 texc2;
        void main() {
            gl_Position = qt_Matrix * qt_Vertex;
            texc1 = qt_MultiTexCoord0;
            texc1.y *= 0.5;
            texc2 = texc1.xy;
            texc2.y += 0.5 + alphaOffset;
        }"
	fragmentShader: "
        varying highp vec2 texc1;
        varying highp vec2 texc2;
        uniform sampler2D source;
        uniform highp float cutOffHeight;
        void main() {
            if(cutOffHeight <= 0.0 || texc1.y > cutOffHeight) {
                lowp float alpha = dot(texture2D(source, texc1.xy).rgb, vec3(0.299, 0.587, 0.114));
                lowp vec3 color = texture2D(source, texc2.xy).rgb;
                gl_FragColor = vec4(color.r, color.g, color.b, alpha);
            }
            else {
                gl_FragColor = vec4(0, 0, 0, 0.0);
            }
        }"
}
