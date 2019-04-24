struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
};

float4 main(VS_OUT input) : SV_Target
{
	float3 objectColor = float3(0.5, 0.5, 0.5);
	float3 lightPos = float3(100.0, 400.0, -200.0);
	float3 lightVector = lightPos;
	float3 normal = input.normal.xyz;
	if (abs(normal.x) < 0.57735) normal.x = 0.0;
	if (abs(normal.y) < 0.57735) normal.y = 0.0;
	if (abs(normal.z) < 0.57735) normal.z = 0.0;
	normal = normalize(normal);
	float3 color = objectColor * 0.2;
	color += max(dot(normal, normalize(lightVector)), 0.0) * objectColor;
	
	return float4(color, 1.0);
}
