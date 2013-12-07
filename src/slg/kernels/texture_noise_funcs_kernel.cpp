#include <string>
namespace slg { namespace ocl {
std::string KernelSource_texture_noise_funcs = 
"#line 2 \"texture_blender_funcs.cl\"\n"
"\n"
"/***************************************************************************\n"
" * Copyright 1998-2013 by authors (see AUTHORS.txt)                        *\n"
" *                                                                         *\n"
" *   This file is part of LuxRender.                                       *\n"
" *                                                                         *\n"
" * Licensed under the Apache License, Version 2.0 (the \"License\");         *\n"
" * you may not use this file except in compliance with the License.        *\n"
" * You may obtain a copy of the License at                                 *\n"
" *                                                                         *\n"
" *     http://www.apache.org/licenses/LICENSE-2.0                          *\n"
" *                                                                         *\n"
" * Unless required by applicable law or agreed to in writing, software     *\n"
" * distributed under the License is distributed on an \"AS IS\" BASIS,       *\n"
" * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*\n"
" * See the License for the specific language governing permissions and     *\n"
" * limitations under the License.                                          *\n"
" ***************************************************************************/\n"
"\n"
"#ifndef M_PI\n"
"#define M_PI 3.14159265358979323846f\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// Texture utility functions\n"
"//------------------------------------------------------------------------------\n"
"\n"
"// Perlin Noise Data\n"
"#define NOISE_PERM_SIZE 256\n"
"__constant int NoisePerm[2 * NOISE_PERM_SIZE] = {\n"
"	151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96,\n"
"	53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,\n"
"	// Rest of noise permutation table\n"
"	8, 99, 37, 240, 21, 10, 23,\n"
"	190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,\n"
"	88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,\n"
"	77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,\n"
"	102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,\n"
"	135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,\n"
"	5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,\n"
"	223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,\n"
"	129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,\n"
"	251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,\n"
"	49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,\n"
"	138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,\n"
"	151, 160, 137, 91, 90, 15,\n"
"	131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,\n"
"	190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,\n"
"	88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,\n"
"	77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,\n"
"	102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,\n"
"	135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,\n"
"	5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,\n"
"	223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,\n"
"	129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,\n"
"	251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,\n"
"	49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,\n"
"	138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180\n"
"};\n"
"\n"
"float Grad(int x, int y, int z, float dx, float dy, float dz) {\n"
"	const int h = NoisePerm[NoisePerm[NoisePerm[x] + y] + z] & 15;\n"
"	const float u = h < 8 || h == 12 || h == 13 ? dx : dy;\n"
"	const float v = h < 4 || h == 12 || h == 13 ? dy : dz;\n"
"	return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);\n"
"}\n"
"\n"
"float NoiseWeight(float t) {\n"
"	const float t3 = t * t * t;\n"
"	const float t4 = t3 * t;\n"
"	return 6.f * t4 * t - 15.f * t4 + 10.f * t3;\n"
"}\n"
"\n"
"float Noise(float x, float y, float z) {\n"
"	// Compute noise cell coordinates and offsets\n"
"	int ix = Floor2Int(x);\n"
"	int iy = Floor2Int(y);\n"
"	int iz = Floor2Int(z);\n"
"	const float dx = x - ix, dy = y - iy, dz = z - iz;\n"
"	// Compute gradient weights\n"
"	ix &= (NOISE_PERM_SIZE - 1);\n"
"	iy &= (NOISE_PERM_SIZE - 1);\n"
"	iz &= (NOISE_PERM_SIZE - 1);\n"
"	const float w000 = Grad(ix, iy, iz, dx, dy, dz);\n"
"	const float w100 = Grad(ix + 1, iy, iz, dx - 1, dy, dz);\n"
"	const float w010 = Grad(ix, iy + 1, iz, dx, dy - 1, dz);\n"
"	const float w110 = Grad(ix + 1, iy + 1, iz, dx - 1, dy - 1, dz);\n"
"	const float w001 = Grad(ix, iy, iz + 1, dx, dy, dz - 1);\n"
"	const float w101 = Grad(ix + 1, iy, iz + 1, dx - 1, dy, dz - 1);\n"
"	const float w011 = Grad(ix, iy + 1, iz + 1, dx, dy - 1, dz - 1);\n"
"	const float w111 = Grad(ix + 1, iy + 1, iz + 1, dx - 1, dy - 1, dz - 1);\n"
"	// Compute trilinear interpolation of weights\n"
"	const float wx = NoiseWeight(dx);\n"
"	const float wy = NoiseWeight(dy);\n"
"	const float wz = NoiseWeight(dz);\n"
"	const float x00 = Lerp(wx, w000, w100);\n"
"	const float x10 = Lerp(wx, w010, w110);\n"
"	const float x01 = Lerp(wx, w001, w101);\n"
"	const float x11 = Lerp(wx, w011, w111);\n"
"	const float y0 = Lerp(wy, x00, x10);\n"
"	const float y1 = Lerp(wy, x01, x11);\n"
"	return Lerp(wz, y0, y1);\n"
"}\n"
"\n"
"float Noise3(const float3 P) {\n"
"	return Noise(P.x, P.y, P.z);\n"
"}\n"
"\n"
"float FBm(const float3 P, const float omega, const int maxOctaves) {\n"
"	// Compute number of octaves for anti-aliased FBm\n"
"	const float foctaves = (float)maxOctaves;\n"
"	const int octaves = Floor2Int(foctaves);\n"
"	// Compute sum of octaves of noise for FBm\n"
"	float sum = 0.f, lambda = 1.f, o = 1.f;\n"
"	for (int i = 0; i < octaves; ++i) {\n"
"		sum += o * Noise3(lambda * P);\n"
"		lambda *= 1.99f;\n"
"		o *= omega;\n"
"	}\n"
"	const float partialOctave = foctaves - (float)octaves;\n"
"	sum += o * SmoothStep(.3f, .7f, partialOctave) *\n"
"			Noise3(lambda * P);\n"
"	return sum;\n"
"}\n"
"\n"
"float Turbulence(const float3 P, const float omega, const int maxOctaves) {\n"
"	// Compute number of octaves for anti-aliased FBm\n"
"	const float foctaves = (float)maxOctaves;\n"
"	const int octaves = Floor2Int(foctaves);\n"
"	// Compute sum of octaves of noise for turbulence\n"
"	float sum = 0.f, lambda = 1.f, o = 1.f;\n"
"	for (int i = 0; i < octaves; ++i) {\n"
"		sum += o * fabs(Noise3(lambda * P));\n"
"		lambda *= 1.99f;\n"
"		o *= omega;\n"
"	}\n"
"	const float partialOctave = foctaves - (float)(octaves);\n"
"	sum += o * SmoothStep(.3f, .7f, partialOctave) *\n"
"	       fabs(Noise3(lambda * P));\n"
"\n"
"	// finally, add in value to account for average value of fabsf(Noise())\n"
"	// (~0.2) for the remaining octaves...\n"
"	sum += (maxOctaves - foctaves) * 0.2f;\n"
"\n"
"	return sum;\n"
"}\n"
"\n"
"static float tex_sin(float a) {\n"
"    a = 0.5f + 0.5f * sin(a);\n"
"    return a;\n"
"}\n"
"\n"
"static float tex_saw(float a) {\n"
"    const float b = 2.f * M_PI;\n"
"    int n = (int) (a / b);\n"
"    a -= n*b;\n"
"    if (a < 0.f) a += b;\n"
"    return a / b;\n"
"}\n"
"\n"
"static float tex_tri(float a) {\n"
"    const float b = 2.f * M_PI;\n"
"    const float rmax = 1.f;\n"
"    a = rmax - 2.f * fabs(floor((a * (1.0 / b)) + 0.5f) - (a * (1.f / b)));\n"
"    return a;\n"
"}\n"
; } }
