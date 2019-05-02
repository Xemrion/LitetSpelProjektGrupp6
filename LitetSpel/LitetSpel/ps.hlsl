Texture2D geometryTexture : register(t0);
Texture2D depthbuffer : register(t1);
TextureCube skybox : register(t2);
TextureCube radianceMap : register(t3);
TextureCube irradianceMap : register(t4);
SamplerState samp;

struct VS_OUT
{
	float4 pos : SV_POSITION;
};

struct Sphere {
	float4 centerRadius;
};
#define MAX_SPHERES 15
cbuffer SphereBuffer {
	Sphere spheres[MAX_SPHERES];
	float4 sphereColor;
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
	return ((mb.w * mb.w) / dot(d, d));
}

float testScene(float3 p)
{
	float mbDist = 0;
	for (int i = 0; i < MAX_SPHERES; ++i)
	{
		mbDist += mb(p, spheres[i].centerRadius);
	}

	return (2.5) - mbDist;
}

float castRay(float3 ro, float3 rd, out bool intersect)
{
	float dist = -ro.z - (10 * rd.z);
	float i;
	float maxDist = -ro.z + (20 / rd.z);
	intersect = false;

	/* calculate ray entry */
	for (i = 0.0; i < 20; i += 1.0)
	{
		float currentDist = testScene(ro + rd * dist);
		dist += currentDist + 0.1;
		if (dist > maxDist) {
			break;
		}
		else if (currentDist < 0.1) {
			intersect = true;
			break;
		}
	}

	/* calculate ray exit */
	//if (intersect) {
	//	back = maxDist;
	//	for (i = 0.0; i < 10; i += 1.0)
	//	{
	//		float currentDist = testScene(ro + rd * back);
	//		back -= currentDist + 0.1;
	//		if (currentDist < 0.1) {
	//			break;
	//		}
	//	}
	//}
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


float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
	return F0 + (max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
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
	float dist = castRay(ro, rd, intersect);
	float3 color = float3(0.0, 0.0, 0.0);
	float3 parallax = (ro * 0.001);

	if (!intersect) {
		color = geometryTexture.Sample(samp, uv);

		if (length(color) <= 0.01) {
			//float angle = 3.1416 * -0.3;
			//float3x3 rotationMatrix = float3x3(
			//	cos(angle), 0, sin(angle),
			//	0, 1, 0,
			//	-sin(angle),0, cos(angle)
			//	);

			//color = skybox.Sample(samp, parallax + mul(rd, rotationMatrix));
			
			//color = skybox.Sample(samp, parallax + rd);
			color = radianceMap.Sample(samp, parallax + rd);
			color *= radianceMap.Sample(samp, parallax + rd);
			color += 0.5;
		}
	}
	else {
		/* depth test */
		float4 projected = mul(float4(ro + rd * dist, 1.0), viewProj);
		projected.xyz /= projected.w;

		if (projected.z > depthbuffer.Sample(samp, uv).x) {
			color = geometryTexture.Sample(samp, uv);
		}
		else {
			/* lighting */
			float3 p = ro + rd * dist;
			float3 normal = calcNormal(p);
			float3 objectColor = float3(1.0, 1.0, 1.0);
			objectColor = float3(1,1,1) - sphereColor.rgb;
			float3 irradiance = irradianceMap.Sample(samp, parallax + normal);
			float3 specularity;
			float3 diffuseColor = float3(0, 0, 0);
			float3 specularColor = float3(0, 0, 0);
			float3 refractDir = refract(rd, normal, 1.00029 / 1.45);
			float back = castRay(p + refractDir * 12, -refractDir, intersect);
			float3 backp = (p + refractDir * 12) - (refractDir * back);
			float3 backNormal = calcNormal(backp);
			
			/* absorbtion diffuse */
			float3 absorbed = exp2(-clamp(sphereColor, 0, 1) * back);
			//diffuseColor = irradiance * absorbed;
			diffuseColor += irradiance * objectColor;

			specularity = fresnelSchlickRoughness(max(dot(normal, rd), 0.0), sphereColor.rgb * (1 - sphereColor.a), sphereColor.a);

			/* refraction diffuse */
			//float3 refractSamplePoint = p + refractDir * (back - dist);
			//refractSamplePoint.xy = (refractSamplePoint.xy / resolution) + 0.5;
			//refractSamplePoint.y = 1 - refractSamplePoint.y;
			//float3 refractColor = geometryTexture.Sample(samp, refractSamplePoint.xy) * absorbed;
			float3 refractColor = float3(0.0, 0.0, 0.0);
			if (length(refractColor) <= 0.00001) {
				refractColor += skybox.Sample(samp, parallax + backp * 0.01 + refract(refractDir, backNormal, 1.45 / 1.00029)) * absorbed;
				refractColor *= irradianceMap.Sample(samp, parallax + backp * 0.01 + refract(refractDir, backNormal, 1.45 / 1.00029)) * absorbed;
			}

			/* specular */
			float3 reflectDir = reflect(rd, normal);
			if (reflectDir.z > 0.010) {
				float3 reflectSamplePoint = p + reflectDir * 300.0;
				reflectSamplePoint.xy = (reflectSamplePoint.xy / resolution) + 0.5;
				reflectSamplePoint.y = 1 - reflectSamplePoint.y;
				specularColor = geometryTexture.Sample(samp, reflectSamplePoint.xy);
			}
			if (length(specularColor) <= 0.00001) {
				specularColor += radianceMap.Sample(samp, parallax + reflectDir);
			}

			/* final color */
			color = (1.0 - specularity) * diffuseColor + specularity * specularColor + refractColor;
		}
	}

	color = color / (color + float3(1.0, 1.0, 1.0));
	return float4(pow(color, 1.0 / 2.2), 1.0);
}
