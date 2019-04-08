struct VS_OUT
{
	float4 pos : SV_POSITION;
};


float4 main(VS_OUT input) : SV_Target
{
	float2 resolution = float2(1280, 720);
	float2 uv = input.pos.xy / resolution;
	float4 color = float4(uv, 0.0, 1.0);

	return color;
}
