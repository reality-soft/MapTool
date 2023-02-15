#include "LevelHeader.hlsli"





[maxvertexcount(3)]
void GS(triangle VS_OUT input[3], inout TriangleStream<VS_OUT> output)
{
	VS_OUT gs_out = (VS_OUT)0;

	for (int i = 0; i < 3; ++i)
	{
		gs_out = input[i];
		output.Append(gs_out);
	}
}