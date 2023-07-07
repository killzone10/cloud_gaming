/**
* The Vienna Vulkan Engine
*
* (c) bei Helmut Hlavacs, University of Vienna
*
*/


#include "VEInclude.h"


namespace ve {

	// the global variables which are needed for the game logic
	double g_time = 10;				
	bool g_gameLost = false;			
	bool g_restart = false;			
	bool g_win = false;
	std::string choice{};
	double pos[2] = { 0 };
	unsigned int id{ 0 };
	double score{ 0 };
	struct nk_color background;
	int clickX = -1;
	int clickY = -1;
	//
	//Zeichne das GUI
	//

	


	glm::vec4 convertTovec4(nk_color nkColor) {
		glm::vec4 colorVec;

		colorVec.r = (float)nkColor.r/255.0;
		colorVec.g = (float)nkColor.g/255.0;
		colorVec.b = (float)nkColor.b/255.0;
		colorVec.a = (float)nkColor.a/255.0;

		return colorVec;
	}

	class EventListenerGUI : public VEEventListener {
	protected:
		
		virtual void onDrawOverlay(veEvent event) {
			VESubrender_Nuklear * pSubrender = (VESubrender_Nuklear*)getEnginePointer()->getRenderer()->getOverlay();
			VESceneNode* Car, * dynamic{ nullptr };
			VESceneNode* random;
			if (pSubrender == nullptr) return;

			struct nk_context * ctx = pSubrender->getContext();
			struct nk_color background {0.5,0.5, 0.5, 0.5 };
			

			Car = getSceneManagerPointer()->getSceneNode("Car");
		

			if (!g_gameLost && !g_win) {

				if (clickX > -1 || clickY > -1) {
					nk_input_begin(ctx);
					//click
					nk_input_motion(ctx, clickX, clickY);
					nk_input_button(ctx, nk_buttons::NK_BUTTON_LEFT, clickX, clickY,1);
					clickX = -1;
					clickY = -1;
				}
				if (nk_begin(ctx, "", nk_rect(0, 0, 200, 200), NK_WINDOW_BORDER )) {
					char outbuffer[100];

					int a = 100;
					int* len = &a;
					score = g_time * 100;

					nk_layout_row_dynamic(ctx, 45, 1);
					Car->getChildrenList();
					if (nk_button_label(ctx, "RotateLeft")) {
						Car->setTransform(glm::rotate(Car->getTransform(), -0.4f, glm::vec3(0, 1, 0)));
					}
					if (nk_button_label(ctx, "RotateRight")) {
						Car->setTransform(glm::rotate(Car->getTransform(), 0.4f, glm::vec3(0, 1, 0)));
						// event handling
						
					}
					if (nk_button_label(ctx, "MoveFront")) {
						Car->setTransform(glm::translate(Car->getTransform(), glm::vec3(0.0f, 0.0f, 10.0f)));

						// event handling
					}
					nk_layout_row_dynamic(ctx, 45, 1);
					sprintf(outbuffer, "Time: %004.1lf", g_time);
					nk_label(ctx, outbuffer, NK_TEXT_LEFT);

			

					nk_input_end(ctx);
					
				}
			}
			else {
				if (clickX > -1 || clickY > -1) {
					nk_input_begin(ctx);
					//click
					nk_input_motion(ctx, clickX, clickY);
					nk_input_button(ctx, nk_buttons::NK_BUTTON_LEFT, clickX, clickY, 1);
					clickX = -1;
					clickY = -1;
				}
				if (g_win) {
					if (nk_begin(ctx, "", nk_rect(500, 500, 200, 170), NK_WINDOW_BORDER)) {
						char outbuffer[100];
						nk_layout_row_dynamic(ctx, 30, 1);
						nk_label(ctx, "You have won", NK_TEXT_LEFT);
						nk_layout_row_dynamic(ctx, 30, 1);

						sprintf(outbuffer, "Score: %004.1lf", score);

						nk_label(ctx, outbuffer, NK_TEXT_LEFT);
						if (nk_button_label(ctx, "Restart")) {
							g_restart = true;
						}
					}
				}
				else {
					if (nk_begin(ctx, "", nk_rect(500, 500, 200, 170), NK_WINDOW_BORDER)) {
						nk_layout_row_dynamic(ctx, 45, 1);
						nk_label(ctx, "You have lost", NK_TEXT_LEFT);
						if (nk_button_label(ctx, "Restart")) {
							g_restart = true;
						}
					}
				}
				nk_input_end(ctx);

			};

			nk_end(ctx);
		}

		virtual bool onKeyboard (veEvent event) {
			return true;
		}

	public:
		///Constructor of class EventListenerGUI
		EventListenerGUI(std::string name) : VEEventListener(name) { };

		///Destructor of class EventListenerGUI
		virtual ~EventListenerGUI() {};
	};


	static std::default_random_engine e{ 12345 };					//Für Zufallszahlen
	static std::uniform_real_distribution<> d{ -10.0f, 10.0f };		//Für Zufallszahlen

	//
	// Überprüfen, ob die Kamera die Kiste berührt
	//
	class EventListenerCollision : public VEEventListener {
	protected:
		virtual void onFrameStarted(veEvent event) {

			if (g_restart) {
				score = 0;
				g_gameLost = false;
				g_restart = false;
				id = 0;
				g_win = false;
				g_time = 10;
				VESceneNode* car = getSceneManagerPointer()->getSceneNode("Car");

				car->setTransform(glm::mat4(1.0));
				car->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)));

				getSceneManagerPointer()->getSceneNode("Car")->setPosition(glm::vec3(25.0f, 0.5f, -3.0f));

				return;
			}
			if (g_gameLost) return;


			VESceneNode* finish = getSceneManagerPointer()->getSceneNode("Finish0");
			VESceneNode* finish1 = getSceneManagerPointer()->getSceneNode("Finish1");
			VESceneNode* finish2 = getSceneManagerPointer()->getSceneNode("Finish2");

			std::vector<VESceneNode*>nodeVector{};
			nodeVector.push_back(finish);
			nodeVector.push_back(finish1);
			nodeVector.push_back(finish2);
			glm::vec3 positionCar = getSceneManagerPointer()->getSceneNode("Car")->getPosition();

			for (auto& v : nodeVector) {
				glm::vec3 finishPosition = v->getPosition();
				float distance = glm::length(positionCar - finishPosition);
				std::string s = v->getName();
				unsigned int number = std::atoi(&s.back());
				if (distance < 5 && id == nodeVector.size()) {
					g_win = true;
				}
				else if (distance < 5 && number == id) { 
					id++;
					g_time += 5.0;
					//->setPosition(glm::vec3(d(e), 1.0f, d(e)));
				}
			}

			g_time -= event.dt;
			if (g_time <= 0) {
			 	g_gameLost = true;
			}

		};

	public:
		///Constructor of class EventListenerCollision
		EventListenerCollision(std::string name) : VEEventListener(name) { };

		///Destructor of class EventListenerCollision
		virtual ~EventListenerCollision() {};
	};

	



	///user defined manager class, derived from VEEngine
	class MyVulkanEngine : public VEEngine {
	public:

		MyVulkanEngine(veRendererType type = veRendererType::VE_RENDERER_TYPE_FORWARD, bool debug=false) : VEEngine(type, debug) {};
		~MyVulkanEngine() {};


		///Register an event listener to interact with the user
		
		virtual void registerEventListeners() {
			VEEngine::registerEventListeners();

			registerEventListener(new EventListenerCollision("Collision"), { veEvent::VE_EVENT_FRAME_STARTED });
			registerEventListener(new EventListenerGUI("GUI"),{ veEvent::VE_EVENT_DRAW_OVERLAY});
		};


		///Load the first level into the game engine
		///The engine uses Y-UP, Left-handed
		virtual void loadLevel( uint32_t numLevel=1) {

			VEEngine::loadLevel(numLevel );			//create standard cameras and lights


			// change camera angle for the game //
			float angle = 0.3;
			glm::vec4 rot4 = glm::vec4(1.0); //total rotation around the axes, is 4d !
			VECamera* pCamera = getSceneManagerPointer()->getCamera();
			VESceneNode* pParent = pCamera->getParent();
			rot4 = glm::vec4(0, 0.0, -1, 1.0);
			glm::vec3 rot3 = glm::vec3(rot4.x, rot4.y, rot4.z);
			glm::mat4 rotate = glm::rotate(glm::mat4(1.0), angle, rot3);
			pCamera->multiplyTransform(rotate);
			rot4 = glm::vec4(1.0, 0, 0, 1.0);
			rot3 = glm::vec3(rot4.x, rot4.y, rot4.z);
			rotate = glm::rotate(glm::mat4(1.0), static_cast<float>(1.0), rot3);
			pCamera->multiplyTransform(rotate);
			rot4 = glm::vec4(0.0, -1.0, 0, 1.0);
			rot3 = glm::vec3(rot4.x, rot4.y, rot4.z);
			rotate = glm::rotate(glm::mat4(1.0), static_cast<float>(0.8), rot3);
			pCamera->multiplyTransform(rotate);
			auto  a = getEnginePointer();

			VESceneNode *pScene;
			VECHECKPOINTER( pScene = getSceneManagerPointer()->createSceneNode("Level 1", getRoot()) );
	
			//scene models

			VESceneNode *sp1;
			VECHECKPOINTER( sp1 = getSceneManagerPointer()->createSkybox("The Sky", "media/models/test/sky/cloudy",
										{	"bluecloud_ft.jpg", "bluecloud_bk.jpg", "bluecloud_up.jpg", 
											"bluecloud_dn.jpg", "bluecloud_rt.jpg", "bluecloud_lf.jpg" }, pScene)  );

			VESceneNode *e4;
			VECHECKPOINTER( e4 = getSceneManagerPointer()->loadModel("The Plane", "media/models/test/plane", "plane_t_n_s.obj",0, pScene) );
			e4->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1.0f, 1000.0f)));

			VEEntity *pE4;
			VECHECKPOINTER( pE4 = (VEEntity*)getSceneManagerPointer()->getSceneNode("The Plane/plane_t_n_s.obj/plane/Entity_0") );
			pE4->setParam( glm::vec4(1000.0f, 1000.0f, 0.0f, 0.0f) );

			VESceneNode *e1, *e2, * e3, *meta, *camera;

		
			VECHECKPOINTER(e2 = getSceneManagerPointer()->loadModel("Finish0", "media/models/test/meta1", "finish2.obj", 0, pScene));
			e2->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 50.0f, 50.0f)));
			e2->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(28.0f, 0.5f, 34.0f)));

			VECHECKPOINTER(e3 = getSceneManagerPointer()->loadModel("Finish1", "media/models/test/meta1", "finish2.obj", 0, pScene));
			e3->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 50.0f, 50.0f)));
			e3->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.5f, 20.0f)));


			VECHECKPOINTER(meta = getSceneManagerPointer()->loadModel("Finish2", "media/models/test/meta2", "finish2.obj", 0, pScene));
			meta->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 50.0f, 50.0f)));
			meta->setTransform(glm::rotate(meta->getTransform(), 1.5707f, glm::vec3(0, 1, 0)));

			meta->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(12.0f, 0.5f, -8.0f)));


			VESceneNode *car;
			VECHECKPOINTER(car = getSceneManagerPointer()->loadModel("Car", "media/models/test/car", "car2.obj", 0,pScene));
			car->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)));
			car->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(25.0f, 0.5f, -3.0f)));

			VESceneNode* road;
			VECHECKPOINTER(road = getSceneManagerPointer()->loadModel("turn1", "media/models/test/curve", "myRoad.obj", 0, pScene));
			road->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)));
			road->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(25.0f, 0.5f, -3.0f)));

			VESceneNode* road1;
			VECHECKPOINTER(road1 = getSceneManagerPointer()->loadModel("turn2", "media/models/test/curve2", "myRoad.obj", 0, pScene));

			road1->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)));

			road1->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, -3.0f)));

			VESceneNode* road2;
			VECHECKPOINTER(road2 = getSceneManagerPointer()->loadModel("turn3", "media/models/test/curve3", "myRoad.obj", 0, pScene));
			road2->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)));
			road2->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(25.0f, 0.5f, 45.0f)));

			VESceneNode* road3;
			VECHECKPOINTER(road3 = getSceneManagerPointer()->loadModel("turn4", "media/models/test/curve4", "myRoad.obj", 0, pScene));
			road3->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)));
			road3->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 45.0f)));

			
		};
	};


}

using namespace ve;

int main() {
	bool debug = true;

	MyVulkanEngine mve(veRendererType::VE_RENDERER_TYPE_FORWARD, debug);	//enable or disable debugging (=callback, validation layers)

	mve.initEngine();
	mve.loadLevel(1);
	mve.run();



	return 0;
}
