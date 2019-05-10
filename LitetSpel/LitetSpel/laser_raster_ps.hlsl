struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

float4 main(VS_OUT input) : SV_Target
{
	float4 color = input.color;
	color.a = 1.0 - abs(color.a);
	color.a *= color.a * color.a;
	
	return color;
}
