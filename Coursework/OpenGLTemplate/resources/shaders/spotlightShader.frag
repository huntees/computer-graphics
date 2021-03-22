#version 400 core 

in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader

uniform sampler2D sampler0;  // The texture sampler
uniform samplerCube CubeMapTex;

uniform bool fogOn;
uniform float discardTime;

out vec4 vOutputColour;

in vec3 n;
in vec4 p;

struct LightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
	vec3 direction;
	float exponent;
	float cutoff;
};

struct MaterialInfo
{
	vec3 Ma;
	vec3 Md;
	vec3 Ms;
	float shininess;
};

uniform LightInfo light1; 
uniform LightInfo pointlight; 
uniform LightInfo spotlight[62]; 

uniform MaterialInfo material1; 

in vec3 worldPosition;
uniform bool renderSkybox;
uniform bool renderTrack;
uniform bool showTrack;


// This function implements the Phong shading model
// The code is based on the OpenGL 4.0 Shading Language Cookbook, pp. 67 - 68, with a few tweaks. 
// Please see Chapter 2 of the book for a detailed discussion.
vec3 PhongModel(vec4 p, vec3 n)
{
	vec3 s = normalize(vec3(light1.position - p));
	vec3 v = normalize(-p.xyz);
	vec3 r = reflect(-s, n);
	vec3 ambient = light1.La * material1.Ma;
	float sDotN = max(dot(s, n), 0.0);
	vec3 diffuse = light1.Ld * material1.Md * sDotN;
	vec3 specular = vec3(0.0);

	vec3 h = normalize(v + s);

	if (sDotN > 0.0)
		specular = light1.Ls * material1.Ms * pow(max(dot(h, n), 0.0), material1.shininess);
	
	return ambient + diffuse + specular;

}

vec3 m_s;
float m_dist;
float m_angle;
float m_cutoff;

float m_spotFactor;
vec3 m_v;
vec3 m_h;
float m_sDotN;
vec3 m_diffuse;
vec3 m_specular;

vec3 BlinnPhongSpotlightModel(LightInfo light, vec4 p, vec3 n)
{
	m_s = normalize(vec3(light.position - p));
	m_dist = length(vec3(p - light.position));
	m_angle= acos(dot(-m_s, light.direction));
	m_cutoff = radians(clamp(light.cutoff, 0.0, 90.0));

	if (m_angle < m_cutoff) {
		m_spotFactor = pow(dot(-m_s, light.direction), light.exponent);
		m_v = normalize(-p.xyz);
		m_h = normalize(m_v + m_s);
		m_sDotN = max(dot(m_s, n), 0.0);
		m_diffuse = light.Ld * m_sDotN;
		m_specular = vec3(0.0);

		if (m_sDotN > 0.0) {
			m_specular = light.Ls * pow(max(dot(m_h, n), 0.0), material1.shininess);
		}

	return (m_spotFactor * ((m_diffuse + m_specular)  / (m_dist * 0.008)));
	}
	else {
		return vec3(0);
	}
}

vec3 PointlightModel(LightInfo light, vec4 p, vec3 n)
{
	vec3 s = normalize(vec3(light.position - p));
	float dist = length(vec3(light.position - p));
	vec3 v = normalize(-p.xyz);
	vec3 r = reflect(-s, n);

	float sDotN = max(dot(s, n), 0.0);
	vec3 diffuse = light.Ld * material1.Md * sDotN;
	vec3 specular = vec3(0.0);

	vec3 h = normalize(v + s);

	if (sDotN > 0.0)
		specular = light.Ls * material1.Ms * pow(max(dot(h, n), 0.0), material1.shininess);
	
	return (diffuse + specular) / (dist * 0.1);

}


void main()
{	

	if (renderSkybox) {
		vOutputColour = texture(CubeMapTex, worldPosition);

	} else if(renderTrack) {

		vec4 vTexColour = texture(sampler0, vTexCoord);	

		if(!showTrack) {
			if (vTexColour.r < fract(discardTime/5)) {
				discard;
			}
		}
		
		vec3 normalised_n = normalize(n);
		vec3 vColour = PhongModel(p, normalised_n);

		vColour += PointlightModel(pointlight, p, normalised_n);

		for (int i = 0 ; i < 63 ; i++) { 
			vColour += BlinnPhongSpotlightModel(spotlight[i], p, normalised_n);
		}

		vOutputColour = vTexColour*vec4(vColour, 1);

	} else {

		vec3 normalised_n = normalize(n);
		vec3 vColour = PhongModel(p, normalised_n);

		vColour += PointlightModel(pointlight, p, normalised_n);

		for (int i = 0 ; i < 62 ; i++) { 
			vColour += BlinnPhongSpotlightModel(spotlight[i], p, normalised_n);
		}

		vec4 vTexColour = texture(sampler0, vTexCoord);	

		vOutputColour = vTexColour*vec4(vColour, 1);

	}

	if(fogOn){

		float dist = length(p.xyz);
		float w = exp(-0.0007f * dist);

		vOutputColour.rgb = mix(vec3(0.5), vOutputColour.rgb, w);
	}
}
