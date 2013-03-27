#include "WorldDrawer2d.h"

//static member init
CoordinateSystem2d WorldDrawer2d::cs_basis;
std::vector<CoordinateSystem2d*> WorldDrawer2d::cs_used;


/*
Functia deseneaza linii in functie de un vector de puncte si o topologie data.
*/

void WorldDrawer2d::drawLine(std::vector<Point2d> points, std::vector<int> topology){
	int j,l;
	for (unsigned int k = 0; k < topology.size(); k+=2){
		j = topology[k];
		l = topology[k+1];

		glDisable(GL_LINE_SMOOTH);
		glLineWidth(5.0f);

		glBegin(GL_LINES);
		glVertex3f(points[j].x, points[j].y, 0);
		glVertex3f(points[l].x, points[l].y, 0);
		glEnd();
	}
}

/*
Functie care deseneaza un cerc de o raza data, folosind GL_TRIANGLE_FAN.
*/

void WorldDrawer2d::drawCircle(std::vector<Point2d> points, float radius){
	for (unsigned int k = 0; k < points.size(); k++){
		glBegin(GL_TRIANGLE_FAN);
							glVertex3f(points[k].x, points[k].y,0);
							for (float angle = 0.0f; angle <= 360.0f; angle = angle + 1.0f)
								glVertex3f(points[k].x + static_cast<float>(sin(angle)) * radius,points[k].y + static_cast<float>(cos(angle)) * radius,0.0f);
							glEnd();
	}
}


void WorldDrawer2d::idleCallbackFunction(){
	//call client function
	onIdle();
	//redisplay
	glutPostRedisplay();
}

void WorldDrawer2d::reshapeCallbackFunction(int w, int h){
	glViewport(0,0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect = (float)w/(float)h;
	gluPerspective(90.0f, aspect, 0.1f, 300.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, 20.0,0.0, 0.0, 0.0,	0.0, 1.0, 0.0);  //looking at xoy
}

void WorldDrawer2d::displayCallbackFunction(){
	//Render objects
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	if(cs_basis.draw_axis){
		//draw basis coord system
		glBegin(GL_LINES);
			glColor3f(0,1,1);
			glVertex3f(-cs_basis.axisup.x,-cs_basis.axisup.y,0.01f);
			glVertex3f(cs_basis.axisup.x,cs_basis.axisup.y,0.01f);
			glColor3f(1,0,1);
			glVertex3f(-cs_basis.axisright.x,-cs_basis.axisright.y,0.01f);
			glVertex3f(cs_basis.axisright.x,cs_basis.axisright.y,0.01f);
		glEnd();
	}

	//draw each used coord system
	for(unsigned int i=0;i<cs_used.size();i++){
		//draw used coord system
		if(cs_used[i]->draw_axis){
			glBegin(GL_LINES);
				glColor3f(0,1,0);
				glVertex3f(cs_used[i]->axiscenter.x,cs_used[i]->axiscenter.y,0.01f);
				glVertex3f(cs_used[i]->axisup.x,cs_used[i]->axisup.y,0.01f);
				glColor3f(1,0,0);
				glVertex3f(cs_used[i]->axiscenter.x,cs_used[i]->axiscenter.y,0.01f);
				glVertex3f(cs_used[i]->axisright.x,cs_used[i]->axisright.y,0.01f);
			glEnd();
		}

		//draw objects
		for(unsigned int j=0;j<cs_used[i]->objects.size();j++){
			//set object color
			glColor3f(cs_used[i]->objects[j]->colorx,cs_used[i]->objects[j]->colory,cs_used[i]->objects[j]->colorz);

			//get data
			std::vector<Point2d> points = cs_used[i]->objects[j]->points;
			std::vector<int> topology = cs_used[i]->objects[j]->topology;
			
			//draw
			if ((j == 1) || (j > 26)){
				if (( j < 33) || ( j == 36 ))
				{
					if(topology.size()<2){
						std::cout<<"Folositi triunghiuri, dimensiune minima topologie =3"<<std::endl;
						continue;
					}

				//obiectul
					glBegin(GL_TRIANGLES);
						for(unsigned int k=0;k<topology.size();k++){
							int index=topology[k];
							glVertex3f(points[index].x, points[index].y,0.0f);
							
						}
						glEnd();
				}
				else {
					if (j < 36)
						drawLine(points, topology);
				}
			}
			else 
			{switch(j){
					case 24: 
							drawCircle(points,0.6f);
						break;
					case 25:
							drawCircle(points,4.8f);
						break;
					case 26:
							drawCircle(points,5.0f);
						break;
					case 0:
							drawCircle(points,0.5f);
						break;
					default:
						break;
			}
			if ((j >= 2) && (j <= 23)) 
				drawCircle(points,0.7f);
			}
			
			
			//axele obiectului
			if(cs_used[i]->objects[j]->drawaxis){
				glBegin(GL_LINES);
					glColor3f(0,1,0);
					glVertex3f(cs_used[i]->objects[j]->axiscenter.x,cs_used[i]->objects[j]->axiscenter.y,0.01f);
					glVertex3f(cs_used[i]->objects[j]->axisup.x,cs_used[i]->objects[j]->axisup.y,0.01f);
					glColor3f(1,0,0);
					glVertex3f(cs_used[i]->objects[j]->axiscenter.x,cs_used[i]->objects[j]->axiscenter.y,0.01f);
					glVertex3f(cs_used[i]->objects[j]->axisright.x,cs_used[i]->objects[j]->axisright.y,0.01f);
				glEnd();
			}
		}
	}

	//swap buffers
	glutSwapBuffers();
}
void WorldDrawer2d::keyboardCallbackFunction(unsigned char key, int posx, int posy){
	if(key==KEY_ESC) glutExit();
	//call client function
	onKey(key);
}

WorldDrawer2d::WorldDrawer2d(int argc, char **argv, int windowWidth, int windowHeight, int windowStartX, int windowStartY, std::string windowName){
	//init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(windowWidth,windowHeight);
	glutInitWindowPosition(windowStartX,windowStartY);
	glutCreateWindow(windowName.c_str());
	
	//bind funcs
	glutDisplayFunc(displayCallbackFunction);
	glutReshapeFunc(reshapeCallbackFunction);
	glutIdleFunc(idleCallbackFunction);
	glutKeyboardFunc(keyboardCallbackFunction);

	glClearColor(0.0f,0.0f,0.0f,1);

	//zbuff
	glEnable(GL_DEPTH_TEST);
}
void WorldDrawer2d::run(){
	glutMainLoop();
}
WorldDrawer2d::~WorldDrawer2d(){
}