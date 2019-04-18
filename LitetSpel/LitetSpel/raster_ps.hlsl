struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
};

float4 main(VS_OUT input) : SV_Target
{
	float3 objectColor = float3(0.5, 0.5, 0.5);
	float3 lightPos = float3(100.0, 400.0, -0.0);
	float3 lightVector = lightPos - input.pos.xyz;
	float3 color = max(dot(input.normal.xyz, normalize(lightVector)), 0.0) * objectColor;
	return float4(1.0, 1.0, 1.0, 1.0);
	return float4(pow(color, 1.0/2.2), 1.0);
}
