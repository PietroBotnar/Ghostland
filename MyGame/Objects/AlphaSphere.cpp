#include "AlphaSphere.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

CAlphaSphere::CAlphaSphere()
{	
}

CAlphaSphere::~CAlphaSphere()
{}
 
void CAlphaSphere::Activate(CVector3f position, float radius, CVector3f startColour, CVector3f endColour, bool zap, float zapLevel)
{
	m_isActive = true;
	m_elapsedTime = 0.0f;
	m_totalTime = 10.0f; 

	mpvPosition = position;

	m_startColour = startColour;
	m_endColour = endColour;
	m_zap = zap;
	m_radius = radius;
	m_zapLevel = zapLevel;  // A number between 0 and 1.  0 means no randomness on lines, 1 means full randomness
}

// Update the crossface based on elapsed time
void CAlphaSphere::Update(float dt)
{
	if (m_isActive == false)
		return;

	// Interpolate between start and end colours
	float f = m_elapsedTime / m_totalTime;
	if (f > 1.0f) 
		f = 1.0f;
	m_colour = (1-f) * m_startColour + f * m_endColour;

	// See if we should stop the effect
	m_elapsedTime += dt;
	if (m_elapsedTime > m_totalTime)
		m_isActive = false;

}

// Return a random floating point number between 0 and 1
float CAlphaSphere::Random()
{
	return (rand() / (float) RAND_MAX) * 2.0f - 1.0f;
}

void CAlphaSphere::Render()
{
	if (m_isActive == false)
		return;

	glPushMatrix();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glTranslatef(mpvPosition.x, mpvPosition.y, mpvPosition.z);

		// Draw an alpha-blended sphere with the given colour and radius
		glColor4f(m_colour.x, m_colour.y, m_colour.z, 0.25);
		glutSolidSphere(m_radius*.99, 35, 35);

		// Draw the lightning (zap) effect with the sphere
		if (m_zap) {
			
			// Rotate the lightning a random amount around a random vector
			CVector3f v = CVector3f(Random(), Random(), Random());
			v.Normalise();
			glRotatef(Random()*180, v.x, v.y, v.z);

			// Create a noisy line that is mapped onto the sphere using spherical coordinates http://en.wikipedia.org/wiki/Spherical_coordinate_system
			std::vector<CVector3f> line;
			int N = 100;
			for (int i = 0; i < N; i++) {
				float theta = (i / (float) N) * 2 * M_PI;
				float phi = M_PI / 2 + Random() * m_zapLevel / 2.0f;
				float x = m_radius * cos(theta) * sin(phi);
				float y = m_radius * sin(theta) * sin(phi);
				float z = m_radius * cos(phi);
				line.push_back(CVector3f(x, y, z));
			}

			// Set the colour
			glColor4f(m_colour.x, m_colour.y, m_colour.z, 0.75);
			glLineWidth(2.0f);

			// Draw the line on the sphere K times with different rotations
			float K = 4;
			for (int i = 0; i < K; i++) {
				glRotatef(180 / K, 0, 1, 0);  // Relative positioning
				glBegin(GL_LINE_LOOP);
					for (int i = 0; i < (int) line.size(); i++) 
						glVertex3f(line[i].x, line[i].y, line[i].z);
				glEnd();
			}
		}
	//	glEnable(GL_TEXTURE_2D);
	//	glEnable(GL_LIGHTING);
		//glDisable(GL_BLEND);
	glPopMatrix();

	

}
