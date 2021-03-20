#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>



CCatmullRom::CCatmullRom()
{
	m_vertexCount = 0;
}

CCatmullRom::~CCatmullRom()
{}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 CCatmullRom::Interpolate(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, float t)
{
	float t2 = t * t;
	float t3 = t2 * t;

	glm::vec3 a = p1;
	glm::vec3 b = 0.5f * (-p0 + p2);
	glm::vec3 c = 0.5f * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3);
	glm::vec3 d = 0.5f * (-p0 + 3.0f * p1 - 3.0f * p2 + p3);

	return a + b * t + c * t2 + d * t3;

}


void CCatmullRom::SetControlPoints()
{
	// Set control points (m_controlPoints) here, or load from disk
	m_controlPoints.push_back(glm::vec3(483, 420, 1992)); 
	m_controlPoints.push_back(glm::vec3(420, 360, 1803));
	m_controlPoints.push_back(glm::vec3(336, 300, 1585));
	m_controlPoints.push_back(glm::vec3(230, 280, 1430));
	m_controlPoints.push_back(glm::vec3(-2, 270, 1193));  //5
	m_controlPoints.push_back(glm::vec3(-400, 200, 1000)); 
	m_controlPoints.push_back(glm::vec3(-642, 160, 841));
	m_controlPoints.push_back(glm::vec3(-838, 150, 677));
	m_controlPoints.push_back(glm::vec3(-960, 160, 510)); 
	m_controlPoints.push_back(glm::vec3(-1059, 170, 416)); //10
	m_controlPoints.push_back(glm::vec3(-1156, 190, 217));
	m_controlPoints.push_back(glm::vec3(-1110, 191, 94));
	m_controlPoints.push_back(glm::vec3(-1047, 202, -3)); 
	m_controlPoints.push_back(glm::vec3(-889, 212, -57));
	m_controlPoints.push_back(glm::vec3(-780, 224, -77)); //15
	m_controlPoints.push_back(glm::vec3(-675, 235, -84));
	m_controlPoints.push_back(glm::vec3(-578, 245, -118));
	m_controlPoints.push_back(glm::vec3(-512, 253, -182)); 
	m_controlPoints.push_back(glm::vec3(-530, 260, -307));
	m_controlPoints.push_back(glm::vec3(-623, 238, -408)); //20
	m_controlPoints.push_back(glm::vec3(-680, 207, -487));
	m_controlPoints.push_back(glm::vec3(-726, 189, -566));
	m_controlPoints.push_back(glm::vec3(-693, 170, -619)); 
	m_controlPoints.push_back(glm::vec3(-574, 140, -709)); 
	m_controlPoints.push_back(glm::vec3(-439, 117, -768)); //25
	m_controlPoints.push_back(glm::vec3(-346, 92, -809));
	m_controlPoints.push_back(glm::vec3(-268, 70, -824)); 
	m_controlPoints.push_back(glm::vec3(-210, 65, -836)); 
	m_controlPoints.push_back(glm::vec3(-100, 65, -870));
	m_controlPoints.push_back(glm::vec3(66, 65, -890)); //30
	m_controlPoints.push_back(glm::vec3(300, 65, -979));
	m_controlPoints.push_back(glm::vec3(451, 82, -1025));
	m_controlPoints.push_back(glm::vec3(553, 96, -1061)); 
	m_controlPoints.push_back(glm::vec3(662, 110, -1093));
	m_controlPoints.push_back(glm::vec3(754, 120, -1121)); //35
	m_controlPoints.push_back(glm::vec3(838, 130, -1144));
	m_controlPoints.push_back(glm::vec3(927, 140, -1158));
	m_controlPoints.push_back(glm::vec3(1100, 160, -1132)); 
	m_controlPoints.push_back(glm::vec3(1300, 205, -996));
	m_controlPoints.push_back(glm::vec3(1539, 250, -884)); //40
	m_controlPoints.push_back(glm::vec3(1771, 297, -844));
	m_controlPoints.push_back(glm::vec3(1780, 289, -685));
	m_controlPoints.push_back(glm::vec3(1907, 275, -460)); 
	m_controlPoints.push_back(glm::vec3(1968, 276, -212));
	m_controlPoints.push_back(glm::vec3(1883, 266, 39)); //45
	m_controlPoints.push_back(glm::vec3(1605, 370, 137));
	m_controlPoints.push_back(glm::vec3(1339, 360, 65));
	m_controlPoints.push_back(glm::vec3(1189, 263, -31)); 
	m_controlPoints.push_back(glm::vec3(900, 228, -224));
	m_controlPoints.push_back(glm::vec3(742, 216, -515)); //50
	m_controlPoints.push_back(glm::vec3(294, 199, -554));
	m_controlPoints.push_back(glm::vec3(100, 187, -606));
	m_controlPoints.push_back(glm::vec3(5, 193, -514));
	m_controlPoints.push_back(glm::vec3(-57, 210, -295));
	m_controlPoints.push_back(glm::vec3(0, 230, -32)); //55
	m_controlPoints.push_back(glm::vec3(25, 320, 137));
	m_controlPoints.push_back(glm::vec3(140, 345, 218));
	m_controlPoints.push_back(glm::vec3(175, 60, 465));
	m_controlPoints.push_back(glm::vec3(221, 28, 640));
	m_controlPoints.push_back(glm::vec3(362, 90, 768)); //60
	m_controlPoints.push_back(glm::vec3(488, 188, 490));
	m_controlPoints.push_back(glm::vec3(610, 288, 255));
	m_controlPoints.push_back(glm::vec3(691, 411, -26));
	m_controlPoints.push_back(glm::vec3(828, 459, -529));
	m_controlPoints.push_back(glm::vec3(903, 474, -837)); //65
	m_controlPoints.push_back(glm::vec3(1264, 619, -873));
	m_controlPoints.push_back(glm::vec3(1222, 488, -535));
	m_controlPoints.push_back(glm::vec3(1167, 437, -179));
	m_controlPoints.push_back(glm::vec3(1180, 437, 0));
	m_controlPoints.push_back(glm::vec3(1223, 438, 175));
	m_controlPoints.push_back(glm::vec3(1233, 438, 367));
	m_controlPoints.push_back(glm::vec3(1252, 433, 506)); //70
	m_controlPoints.push_back(glm::vec3(1226, 435, 811));
	m_controlPoints.push_back(glm::vec3(1164, 439, 1161));
	m_controlPoints.push_back(glm::vec3(1029, 445, 1570));
	m_controlPoints.push_back(glm::vec3(926, 445, 1760));
	m_controlPoints.push_back(glm::vec3(863, 448, 1923));
	m_controlPoints.push_back(glm::vec3(703, 438, 2208));



	// Optionally, set upvectors (m_controlUpVectors, one for each control point as well)
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); 
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); //5
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); //10
	m_controlUpVectors.push_back(glm::vec3(1, 1, -0.2));
	m_controlUpVectors.push_back(glm::vec3(1, 1, 1));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); 
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); //15
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); 
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); //20
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(1, 1, 1));
	m_controlUpVectors.push_back(glm::vec3(1, 1, 1));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); 
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); //25
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); 
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); //30
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, -1));
	m_controlUpVectors.push_back(glm::vec3(0, 0, -1));
	m_controlUpVectors.push_back(glm::vec3(0, 0, -1)); 
	m_controlUpVectors.push_back(glm::vec3(0, 1, -1)); //35
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); 
	m_controlUpVectors.push_back(glm::vec3(-1, 1, 0)); //40
	m_controlUpVectors.push_back(glm::vec3(-1, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(-1, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(-1, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(-1, 1, 0)); 
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); //45
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(1, 1, 0)); //1, 1, -1
	m_controlUpVectors.push_back(glm::vec3(-1, 1, 1)); //50
	m_controlUpVectors.push_back(glm::vec3(1, 1, -1));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(1, 1, 1));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); 
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); //55
	m_controlUpVectors.push_back(glm::vec3(1, 1, -1));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(1, 1, 0)); 
	m_controlUpVectors.push_back(glm::vec3(0, 1, -1)); //60
	m_controlUpVectors.push_back(glm::vec3(-1, 0, 0));
	m_controlUpVectors.push_back(glm::vec3(-1, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, -1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, -1, 0)); 
	m_controlUpVectors.push_back(glm::vec3(0, -1, 0)); //65
	m_controlUpVectors.push_back(glm::vec3(0, -1, 0));
	m_controlUpVectors.push_back(glm::vec3(1, -1, 1));
	m_controlUpVectors.push_back(glm::vec3(1, 0, 1)); 
	m_controlUpVectors.push_back(glm::vec3(1, 1, 1)); 
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0)); //70
	m_controlUpVectors.push_back(glm::vec3(-1, 1, -1));
	m_controlUpVectors.push_back(glm::vec3(-1, 0, -1));
	m_controlUpVectors.push_back(glm::vec3(-1, -1, -1));
	m_controlUpVectors.push_back(glm::vec3(0, -1, 0));
	m_controlUpVectors.push_back(glm::vec3(1, -1, 1));

	m_controlUpVectors.push_back(glm::vec3(1, 0, 1));
	m_controlUpVectors.push_back(glm::vec3(1, 1, 1));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
}


// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints()
{
	int M = (int)m_controlPoints.size();

	float fAccumulatedLength = 0.0f;
	m_distances.push_back(fAccumulatedLength);
	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(m_controlPoints[i - 1], m_controlPoints[i]);
		m_distances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(m_controlPoints[M - 1], m_controlPoints[0]);
	m_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CCatmullRom::Sample(float d, glm::vec3& p, glm::vec3& up)
{
	if (d < 0)
		return false;

	int M = (int)m_controlPoints.size();
	if (M == 0)
		return false;


	float fTotalLength = m_distances[m_distances.size() - 1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int)(d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int)m_distances.size() - 1; i++) {
		if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return false;

	// Interpolate on current segment -- get t
	float fSegmentLength = m_distances[j + 1] - m_distances[j];
	float t = (fLength - m_distances[j]) / fSegmentLength;

	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j - 1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;

	// Interpolate to get the point (and upvector)
	p = Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
	if (m_controlUpVectors.size() == m_controlPoints.size())
		up = glm::normalize(Interpolate(m_controlUpVectors[iPrev], m_controlUpVectors[iCur], m_controlUpVectors[iNext], m_controlUpVectors[iNextNext], t));

	return true;
}



// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int numSamples)
{
	glm::vec3 p, up;

	// Compute the lengths of each segment along the control polygon, and the total length
	ComputeLengthsAlongControlPoints();
	float fTotalLength = m_distances[m_distances.size() - 1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / numSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);

	}


	// Repeat once more for truly equidistant points
	m_controlPoints = m_centrelinePoints;
	m_controlUpVectors = m_centrelineUpVectors;
	m_centrelinePoints.clear();
	m_centrelineUpVectors.clear();
	m_distances.clear();
	ComputeLengthsAlongControlPoints();
	fTotalLength = m_distances[m_distances.size() - 1];
	fSpacing = fTotalLength / numSamples;
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}


}



void CCatmullRom::CreateCentreline()
{
	// Call Set Control Points
	SetControlPoints();

	// Call UniformlySampleControlPoints with the number of samples required
	UniformlySampleControlPoints(1000);

	// Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card
	glGenVertexArrays(1, &m_vaoCentreline);
	glBindVertexArray(m_vaoCentreline);

	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();

	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < m_centrelinePoints.size(); i++) {

		vbo.AddData(&m_centrelinePoints[i], sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}

	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));

	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}


void CCatmullRom::CreateOffsetCurves(float width)
{
	// Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively
	for (int i = 0; i < m_centrelinePoints.size(); i++) {
		glm::vec3 p = m_centrelinePoints[i];
		glm::vec3 p_y = m_centrelineUpVectors[i];

		glm::vec3 pNext = m_centrelinePoints[(i + 1) % m_centrelinePoints.size()];

		glm::vec3 T = glm::normalize(pNext - p);
		glm::vec3 N = glm::normalize(glm::cross(T, p_y));
		glm::vec3 B = glm::normalize(glm::cross(N, T));

		glm::vec3 l = p - (width / 2) * N;
		glm::vec3 r = p + (width / 2) * N;
		
		m_leftOffsetPoints.push_back(l);
		m_rightOffsetPoints.push_back(r);
	}

	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
	// Note it is possible to only use one VAO / VBO with all the points instead.


	//---------------------------------Left Offset----------------------------------------
	glGenVertexArrays(1, &m_vaoLeftOffsetCurve);
	glBindVertexArray(m_vaoLeftOffsetCurve);

	// Create a VBO
	CVertexBufferObject vboLeftOffsetCurve;
	vboLeftOffsetCurve.Create();
	vboLeftOffsetCurve.Bind();

	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < m_leftOffsetPoints.size(); i++) {

		vboLeftOffsetCurve.AddData(&m_leftOffsetPoints[i], sizeof(glm::vec3));
		vboLeftOffsetCurve.AddData(&texCoord, sizeof(glm::vec2));
		vboLeftOffsetCurve.AddData(&normal, sizeof(glm::vec3));
	}

	// Upload the VBO to the GPU
	vboLeftOffsetCurve.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));

	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	//---------------------------------Right Offset---------------------------------------
	glGenVertexArrays(1, &m_vaoRightOffsetCurve);
	glBindVertexArray(m_vaoRightOffsetCurve);

	// Create a VBO
	CVertexBufferObject vboRightOffsetCurve;
	vboRightOffsetCurve.Create();
	vboRightOffsetCurve.Bind();

	for (unsigned int i = 0; i < m_rightOffsetPoints.size(); i++) {

		vboRightOffsetCurve.AddData(&m_rightOffsetPoints[i], sizeof(glm::vec3));
		vboRightOffsetCurve.AddData(&texCoord, sizeof(glm::vec2));
		vboRightOffsetCurve.AddData(&normal, sizeof(glm::vec3));
	}

	// Upload the VBO to the GPU
	vboRightOffsetCurve.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	//GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));

	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

}


void CCatmullRom::CreateTrack(string filename)
{

	m_texture.Load(filename);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card

	glGenVertexArrays(1, &m_vaoTrack);
	glBindVertexArray(m_vaoTrack);

	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();


	glm::vec2 texCoordLeft[2] =
	{
		glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f)
	};

	glm::vec2 texCoordRight[2] =
	{
		glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f)
	};

	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < m_leftOffsetPoints.size() + 1; i++) {

		vbo.AddData(&m_leftOffsetPoints[i % m_leftOffsetPoints.size()], sizeof(glm::vec3));
		vbo.AddData(&texCoordLeft[i % 2], sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));

		vbo.AddData(&m_rightOffsetPoints[i % m_leftOffsetPoints.size()], sizeof(glm::vec3));
		vbo.AddData(&texCoordRight[i  % 2], sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));

		m_vertexCount += 2;
	}

	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));

	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}


void CCatmullRom::RenderCentreline()
{
	glLineWidth(2.0f);
	glPointSize(5.0f);

	// Bind the VAO m_vaoCentreline and render it
	glBindVertexArray(m_vaoCentreline);
	glDrawArrays(GL_POINTS, 0, m_centrelinePoints.size());
	glDrawArrays(GL_LINE_LOOP, 0, m_centrelinePoints.size());
}

void CCatmullRom::RenderOffsetCurves()
{
	glLineWidth(2.0f);
	glPointSize(5.0f);

	// Bind the VAO m_vaoLeftOffsetCurve and render it
	glBindVertexArray(m_vaoLeftOffsetCurve);
	glDrawArrays(GL_POINTS, 0, m_leftOffsetPoints.size());
	glDrawArrays(GL_LINE_LOOP, 0, m_leftOffsetPoints.size());

	// Bind the VAO m_vaoRightOffsetCurve and render it
	glBindVertexArray(m_vaoRightOffsetCurve);
	glDrawArrays(GL_POINTS, 0, m_rightOffsetPoints.size());
	glDrawArrays(GL_LINE_LOOP, 0, m_rightOffsetPoints.size());
}


void CCatmullRom::RenderTrack()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoTrack);
	m_texture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexCount);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int CCatmullRom::CurrentLap(float d)
{

	return (int)(d / m_distances.back());

}

glm::vec3 CCatmullRom::_dummy_vector(0.0f, 0.0f, 0.0f);



//Code Duplication here, can refactor the above to take in arguments for multiple splines. In order to not break the above code, the following ones are used 
//for environment ships for the moment.

// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::Env_ComputeLengthsAlongControlPoints()
{
	int M = (int)env_controlPoints.size();

	float fAccumulatedLength = 0.0f;
	env_distances.push_back(fAccumulatedLength);
	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(env_controlPoints[i - 1], env_controlPoints[i]);
		env_distances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(env_controlPoints[M - 1], env_controlPoints[0]);
	env_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CCatmullRom::Env_Sample(float d, glm::vec3& p, glm::vec3& up)
{
	if (d < 0)
		return false;

	int M = (int)env_controlPoints.size();
	if (M == 0)
		return false;


	float fTotalLength = env_distances[env_distances.size() - 1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int)(d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int)env_distances.size() - 1; i++) {
		if (fLength >= env_distances[i] && fLength < env_distances[i + 1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return false;

	// Interpolate on current segment -- get t
	float fSegmentLength = env_distances[j + 1] - env_distances[j];
	float t = (fLength - env_distances[j]) / fSegmentLength;

	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j - 1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;

	// Interpolate to get the point (and upvector)
	p = Interpolate(env_controlPoints[iPrev], env_controlPoints[iCur], env_controlPoints[iNext], env_controlPoints[iNextNext], t);
	if (env_controlUpVectors.size() == env_controlPoints.size())
		up = glm::normalize(Interpolate(env_controlUpVectors[iPrev], env_controlUpVectors[iCur], env_controlUpVectors[iNext], env_controlUpVectors[iNextNext], t));

	return true;
}



// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::Env_UniformlySampleControlPoints(int numSamples)
{
	glm::vec3 p, up;

	// Compute the lengths of each segment along the control polygon, and the total length
	Env_ComputeLengthsAlongControlPoints();
	float fTotalLength = env_distances[env_distances.size() - 1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / numSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < numSamples; i++) {
		Env_Sample(i * fSpacing, p, up);
		env_centrelinePoints.push_back(p);
		if (env_controlUpVectors.size() > 0)
			env_centrelineUpVectors.push_back(up);

	}


	// Repeat once more for truly equidistant points
	env_controlPoints = env_centrelinePoints;
	env_controlUpVectors = env_centrelineUpVectors;
	env_centrelinePoints.clear();
	env_centrelineUpVectors.clear();
	env_distances.clear();
	Env_ComputeLengthsAlongControlPoints();
	fTotalLength = env_distances[env_distances.size() - 1];
	fSpacing = fTotalLength / numSamples;
	for (int i = 0; i < numSamples; i++) {
		Env_Sample(i * fSpacing, p, up);
		env_centrelinePoints.push_back(p);
		if (env_controlUpVectors.size() > 0)
			env_centrelineUpVectors.push_back(up);
	}


}



void CCatmullRom::Env_CreateCentreline()
{
	// Call Set Control Points
	Env_SetControlPoints();

	// Call UniformlySampleControlPoints with the number of samples required
	Env_UniformlySampleControlPoints(500);

	// Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card
	glGenVertexArrays(1, &env_vaoCentreline);
	glBindVertexArray(env_vaoCentreline);

	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();

	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < env_centrelinePoints.size(); i++) {

		vbo.AddData(&env_centrelinePoints[i], sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}

	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));

	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}

void CCatmullRom::Env_SetControlPoints()
{
	// Set control points (m_controlPoints) here, or load from disk
	env_controlPoints.push_back(glm::vec3(1172, 41, -158));
	env_controlPoints.push_back(glm::vec3(345, 44, -626));
	env_controlPoints.push_back(glm::vec3(117, 96, -1045));
	env_controlPoints.push_back(glm::vec3(224, 85, -1210));
	env_controlPoints.push_back(glm::vec3(1274, 127, -567));
	env_controlPoints.push_back(glm::vec3(1413, 234, -732));
	env_controlPoints.push_back(glm::vec3(1774, 284, -1114));
	env_controlPoints.push_back(glm::vec3(2303, 275, -651));
	env_controlPoints.push_back(glm::vec3(2076, 235, -161));
	env_controlPoints.push_back(glm::vec3(1684, 231, 245));
	env_controlPoints.push_back(glm::vec3(1274, 202, 426));
	


	env_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	env_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	env_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	env_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	env_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	env_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	env_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	env_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	env_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	env_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	env_controlUpVectors.push_back(glm::vec3(0, 1, 0));
}