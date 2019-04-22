Texture2D background : register(t0);
Texture2D depthbuffer : register(t1);
SamplerState samp;

struct VS_OUT
{
	float4 pos : SV_POSITION;
};

struct Sphere {
	float4 centerRadius;
};

cbuffer SphereBuffer {
	Sphere spheres[5];
};

cbuffer Camera {
	float3 cameraPos;
};

cbuffer Corners {
	float4 upLeft;
	float4 upRight;
	float4 downLeft;
	float4 downRight;
};

cbuffer Matrices {
	float4x4 viewProj;
};

float testSphere(float3 p, float4 s)
{
	return length(s.xyz - p) - s.w;
}

float mb(float3 p, float4 mb)
{
	float3 d = mb.xyz - p;
	return 0.35 - ((mb.w * mb.w) / dot(mb.xyz - p, mb.xyz - p));
}

float testScene(float3 p)
{
	float d0 = mb(p, spheres[0].centerRadius);
	float d1 = mb(p, spheres[1].centerRadius);
	float d2 = mb(p, spheres[2].centerRadius);
	float d3 = mb(p, spheres[3].centerRadius);
	float d4 = mb(p, spheres[4].centerRadius);
	float mbDist = d0 + d1 + d2 + d3 + d4;

	return mbDist;
}

float castRay(float3 ro, float3 rd, out bool intersect, out float back)
{
	float dist = -ro.z - 10.0;
	float i;
	float maxDist = -ro.z + 60.0;
	intersect = false;

	/* calculate ray entry */
	for (i = 0.0; i < 30; i += 1.0)
	{
		float currentDist = testScene(ro + rd * dist);
		dist += currentDist + 0.05;
		if (dist > maxDist) {
			break;
		}
		else if (currentDist < 0.01) {
			intersect = true;
			break;
		}
	}

	/* calculate ray exit */
	if (intersect) {
		back = dist + 30.0;
		for (i = 0.0; i < 30; i += 1.0)
		{
			float currentDist = testScene(ro + rd * back);
			back -= currentDist + 0.1;
			if (currentDist < 0.01) {
				break;
			}
		}
	}
	return dist;
}

float3 calcNormal(float3 p)
{
	const float h = 0.0001;
	const float2 k = float2(1, -1);
	return normalize(k.xyy * testScene(p + k.xyy * h) +
		k.yyx * testScene(p + k.yyx * h) +
		k.yxy * testScene(p + k.yxy * h) +
		k.xxx * testScene(p + k.xxx * h));
}

float4 main(VS_OUT input) : SV_Target
{
	float2 resolution = float2(1280, 720);
	float2 uv = input.pos.xy / resolution;
	float3 ro = cameraPos;
	float3 rd = normalize(lerp(
		float3(lerp(upLeft.xyz, downLeft.xyz, uv.y)),
		float3(lerp(upRight.xyz, downRight.xyz, uv.y)),
		uv.x));

	bool intersect;
	float back;
	float dist = castRay(ro, rd, intersect, back);
	float3 color = float3(0.0, 0.0, 0.0);

	if (!intersect) {
		color = background.Sample(samp, uv);
	}
	else {
		/* depth test */
		float4 projected = mul(float4(ro + rd * dist, 1.0), viewProj);
		projected.xyz /= projected.w;

		if (projected.z > depthbuffer.Sample(samp, uv).x) {
			color = background.Sample(samp, uv);
		}
		else {
			/* lighting */
			float3 p = ro + rd * dist;
			float3 objectColor = float3(0.3, 0.3, 1.0);
			float3 normal = calcNormal(p);
			float3 lightVector = normalize(float3(0.0, 400.0, 100.0));
			float3 diffuseColor = float3(0, 0, 0);
			float3 specularColor = float3(0, 0, 0);
			
			/* specular */
			specularColor = pow(max(dot(normal, normalize(lightVector - rd)), 0.0), 32) * float3(1.0, 1.0, 1.0);
			
			/* absorbtion diffuse */
			float3 absorbed = exp(-float3(0.35, 0.75, 2.35) * (back - dist));
			diffuseColor = objectColor * absorbed;

			/* refraction diffuse */
			float3 ref = refract(rd, normal, 0.75);
			float3 samplePoint = p + ref * (-500.0 / ref.z);
			float4 projectedSample = mul(float4(samplePoint, 1.0), viewProj);
			samplePoint.xy = (samplePoint.xy / resolution) + 0.5;
			if (projectedSample.z / projectedSample.w > depthbuffer.Sample(samp, samplePoint.xy).x)
			{
				diffuseColor += background.Sample(samp, samplePoint.xy) * absorbed;
			}

			/* final color */
			float diffuseRatio = 0.99;
			color = diffuseRatio * diffuseColor + (1 - diffuseRatio) * specularColor;
		}
	}
	
	return float4(pow(color, 1.0 / 2.2), 1.0);
}
