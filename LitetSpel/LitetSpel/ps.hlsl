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
	float4 color;
};

#define MAX_SPHERES 15
cbuffer SphereBuffer : register(b1) {
	Sphere spheres[MAX_SPHERES];
	int nSpheres;
};

cbuffer Camera : register(b2) {
	float3 cameraPos;
};

cbuffer Corners : register(b3) {
	float4 upLeft;
	float4 upRight;
	float4 downLeft;
	float4 downRight;
};

cbuffer Matrices : register(b4) {
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
	for (int i = 0; i < nSpheres; ++i)
	{
		mbDist += mb(p, spheres[i].centerRadius);
	}

	return (2.5) - mbDist;
}

float castRay(float3 ro, float3 rd, out float minDist)
{
	float dist = -ro.z - (10 * rd.z);
	float i;
	float maxDist = -ro.z + (20 / rd.z);
	minDist = 1000000.0;

	/* calculate ray entry */
	for (i = 0.0; i < 20; i += 1.0)
	{
		float currentDist = testScene(ro + rd * dist);
		dist += currentDist + 0.1;
		minDist = min(currentDist, minDist);

		if (dist > maxDist) {
			break;
		}
		else if (currentDist < 0.1) {
			break;
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

//type: 0 metaball, 1 line
float4 calcColor(float3 p)
{
	float4 color = spheres[0].color;
	float minDist = mb(p, spheres[0].centerRadius);
	for (int i = 1; i < nSpheres; ++i) {
		float dist = mb(p, spheres[i].centerRadius);

		if (dist > minDist) {
			color = spheres[i].color;
			minDist = dist;
		}
	}
	return color;
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

	float minDist;
	float dist = castRay(ro, rd, minDist);
	float4 color = float4(0.0, 0.0, 0.0, 0.0);
	float3 parallax = (ro * 0.001);

	if (minDist > 0.1) {
		color = geometryTexture.Sample(samp, uv);

		if (color.a < 1.0) {
			float4 radiance = radianceMap.Sample(samp, parallax + rd);
			//color.rgb = color.a * color.rgb + float3(1.0,1.0,1.0)*(1.0 - color.a);
			color.rgb += color.a * color.rgb + ((1.0 - color.a) * (radiance.rgb * radiance.rgb + 0.5));
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
			float4 sphereColor = calcColor(p);
			float3 objectColor = float3(1.0, 1.0, 1.0);
			//objectColor = float3(1, 1, 1) - sphereColor.rgb;
			float3 specularity;
			float3 diffuseColor = float3(0, 0, 0);
			float3 specularColor = float3(0, 0, 0);
			float refractiveIndex = 1.5;
			float3 refractDir = refract(rd, normal, 1.00029 / refractiveIndex);
			float back = castRay(p + refractDir * 12, - refractDir, minDist);
			float3 backp = (p + refractDir * 12) - (refractDir * back);
			float3 backNormal = calcNormal(backp);
			float3 backSample = parallax + backp * 0.01 + refract(refractDir, backNormal, refractiveIndex / 1.00029);
			float3 irradiance = irradianceMap.Sample(samp, backSample);
			
			/* absorbtion diffuse */
			float3 absorbed = exp(-sphereColor * back);
			diffuseColor = objectColor * absorbed;

			specularity = fresnelSchlickRoughness(max(dot(normal, rd), 0.0), sphereColor.rgb * sphereColor.a, 1 - sphereColor.a);

			/* refraction diffuse */
			float3 refractColor = float3(0.0, 0.0, 0.0);
			if (length(refractColor) <= 0.00001) {
				refractColor += radianceMap.Sample(samp, backSample) * absorbed;
				refractColor *= irradiance * absorbed;
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
			color.rgb = (1.0 - specularity) * (diffuseColor + refractColor) + specularity * specularColor;
		}
	}
	
	color.rgb = color.rgb / (color.rgb + float3(1.0, 1.0, 1.0));
	return float4(pow(color.rgb, 1.0 / 2.2), 1.0);
}
