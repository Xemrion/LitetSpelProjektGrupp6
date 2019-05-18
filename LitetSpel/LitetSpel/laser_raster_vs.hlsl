struct VS_IN
{
	float3 pos : POSITION;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	linear float4 color : COLOR;
};

cbuffer Transforms {
	float4x4 worldTransforms[100];
	float4 color[100];
};

VS_OUT main(VS_IN input, uint instanceID : SV_InstanceID)
{
	VS_OUT output = (VS_OUT)0;
	output.pos = mul(float4(input.pos, 1.0), worldTransforms[instanceID]);
	output.color = color[instanceID];
	output.color.a = input.pos.y;
	return output;
}
