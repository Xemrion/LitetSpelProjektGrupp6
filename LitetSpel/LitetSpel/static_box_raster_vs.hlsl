struct VS_IN
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
};

struct VS_OUT
{
	float3 worldPos : POSITION;
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR;
};

cbuffer Colors : register(b0) {
	float4 color[300];
};

cbuffer Matrices : register(b1) {
	float4x4 viewProj;
};

VS_OUT main(VS_IN input, uint vertexID : SV_VertexID)
{
	VS_OUT output = (VS_OUT)0;
	output.worldPos = input.pos;
	output.pos = mul(float4(input.pos, 1.0), viewProj);
	output.normal = float4(input.normal, 0.0);
	output.color = color[vertexID / 36];
	return output;
}
