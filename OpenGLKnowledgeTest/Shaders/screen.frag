#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
	// Normal
	FragColor = texture(tex, texCoords);

	// Inverted
	//FragColor = vec4(vec3(1.0 - texture(tex, texCoords)), 1.0);

	// Grayscale
	/*
	FragColor = texture(tex, texCoords);
	//float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;						// True grayscale
	float average = FragColor.r * 0.2126 + FragColor.g * 0.7152 + FragColor.b * 0.0722;		// Realistic grayscale
	FragColor = vec4(average, average, average, 1.0);
	*/

	// KERNEL EFFECTS
	const float offset = 1 / 300.0;

	const vec2 offsets[9] = vec2[] (
		vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right  
	);

	// Sharpen Kernel
	/*
	const float kernel[9] = float[] (
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	);
	*/

	// Blur Kernel
	/*
	const float kernel[9] = float[] (
		1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
		2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
		1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0
	);
	*/

	// Edge-Detection Kernel
	const float kernel[9] = float[] (
		1,  1, 1,
		1, -8, 1,
		1,  1, 1
	);

	// Emboss Kernel
	/*
	const float kernel[9] = float[] (
		-2, -1, 0,
		-1,  1, 1,
		 0,  1, 2
	);
	*/

	vec3 color;
	for (int i = 0; i < 9; ++i) {
		color += vec3(texture(tex, texCoords + offsets[i])) * kernel[i];
	}

	FragColor = vec4(color, 1.0);
}