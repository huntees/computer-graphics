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
	//m_controlPoints.push_back(glm::vec3(100 + 300, 200, 0));
	//m_controlPoints.push_back(glm::vec3(71 + 300, 200, 71));
	//m_controlPoints.push_back(glm::vec3(0 + 300, 200, 100));
	//m_controlPoints.push_back(glm::vec3(-71 + 300, 200, 71));
	//m_controlPoints.push_back(glm::vec3(-100 + 300, 200, 0));
	//m_controlPoints.push_back(glm::vec3(-71 + 300, 200, -71 - 300));
	//m_controlPoints.push_back(glm::vec3(0 + 300, 200, -100 - 300));
	//m_controlPoints.push_back(glm::vec3(71 + 300, 200, -71 - 300));
	m_controlPoints.push_back(glm::vec3(483, 420, 1992));
	m_controlPoints.push_back(glm::vec3(420, 360, 1803));
	m_controlPoints.push_back(glm::vec3(336, 300, 1585));
	m_controlPoints.push_back(glm::vec3(230, 280, 1430));
	m_controlPoints.push_back(glm::vec3(-2, 270, 1193));
	m_controlPoints.push_back(glm::vec3(-250, 229, 1057));
	m_controlPoints.push_back(glm::vec3(-400, 200, 1000));
	m_controlPoints.push_back(glm::vec3(-642, 160, 841));
	m_controlPoints.push_back(glm::vec3(-838, 150, 677));
	m_controlPoints.push_back(glm::vec3(-960, 160, 510));
	m_controlPoints.push_back(glm::vec3(-1059, 170, 416));
	m_controlPoints.push_back(glm::vec3(-1186, 186, 301));
	m_controlPoints.push_back(glm::vec3(-1156, 190, 217));
	m_controlPoints.push_back(glm::vec3(-1110, 191, 94));
	m_controlPoints.push_back(glm::vec3(-1047, 202, -3));
	m_controlPoints.push_back(glm::vec3(-889, 212, -57));
	m_controlPoints.push_back(glm::vec3(-780, 224, -77));
	m_controlPoints.push_back(glm::vec3(-675, 235, -84));
	m_controlPoints.push_back(glm::vec3(-578, 245, -118));
	m_controlPoints.push_back(glm::vec3(-512, 253, -182));
	m_controlPoints.push_back(glm::vec3(-530, 260, -307));
	m_controlPoints.push_back(glm::vec3(-623, 238, -408));
	m_controlPoints.push_back(glm::vec3(-680, 207, -487));
	m_controlPoints.push_back(glm::vec3(-704, 195, -523));
	m_controlPoints.push_back(glm::vec3(-726, 189, -566));
	m_controlPoints.push_back(glm::vec3(-693, 170, -619));
	m_controlPoints.push_back(glm::vec3(-574, 140, -709)); //-13 -22
	m_controlPoints.push_back(glm::vec3(-439, 117, -768));
	m_controlPoints.push_back(glm::vec3(-346, 92, -809));
	m_controlPoints.push_back(glm::vec3(-268, 70, -824)); 
	m_controlPoints.push_back(glm::vec3(-210, 65, -836));
	m_controlPoints.push_back(glm::vec3(-100, 65, -870));
	m_controlPoints.push_back(glm::vec3(66, 65, -890));
	m_controlPoints.push_back(glm::vec3(300, 65, -979));
	m_controlPoints.push_back(glm::vec3(451, 82, -1025));

	//m_controlPoints.push_back(glm::vec3(480, 96, -1039));
	//m_controlPoints.push_back(glm::vec3(552, 100, -1069));
	//m_controlPoints.push_back(glm::vec3(604, 100, -1080));
	//m_controlPoints.push_back(glm::vec3(701, 105, -1100));

	m_controlPoints.push_back(glm::vec3(553, 96, -1061));
	m_controlPoints.push_back(glm::vec3(662, 100, -1093));
	m_controlPoints.push_back(glm::vec3(754, 100, -1121));
	m_controlPoints.push_back(glm::vec3(838, 105, -1144));
	m_controlPoints.push_back(glm::vec3(927, 105, -1158));
	m_controlPoints.push_back(glm::vec3(645, 435, 2280));


	// Optionally, set upvectors (m_controlUpVectors, one for each control point as well)
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
	m_controlUpVectors.push_back(glm::vec3(-1, 0, 0));
	m_controlUpVectors.push_back(glm::vec3(-1, 0, 0));
	m_controlUpVectors.push_back(glm::vec3(-1, 0, 0));
	m_controlUpVectors.push_back(glm::vec3(-1, 0, 0));
	m_controlUpVectors.push_back(glm::vec3(-0.5, 0, 0));
	m_controlUpVectors.push_back(glm::vec3(0, 1, 0));
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
	UniformlySampleControlPoints(500);

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


void CCatmullRom::CreateTrack()
{

	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card

	glGenVertexArrays(1, &m_vaoTrack);
	glBindVertexArray(m_vaoTrack);

	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();

	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < m_leftOffsetPoints.size() + 1; i++) {

		vbo.AddData(&m_leftOffsetPoints[i % m_leftOffsetPoints.size()], sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));

		vbo.AddData(&m_rightOffsetPoints[i % m_leftOffsetPoints.size()], sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoTrack);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexCount);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int CCatmullRom::CurrentLap(float d)
{

	return (int)(d / m_distances.back());

}

glm::vec3 CCatmullRom::_dummy_vector(0.0f, 0.0f, 0.0f);