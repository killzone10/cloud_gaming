/**
* The Vienna Vulkan Engine
*
* (c) bei Helmut Hlavacs, University of Vienna
*
*/


#include "VEInclude.h"


namespace ve {


	double g_time = 10;				
	bool g_gameLost = false;			
	bool g_restart = false;			
	bool g_win = false;
	std::string choice{};
	double pos[2] = { 0 };
	unsigned int id{ 0 };
	double score{ 0 };
	struct nk_color background;

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

			//Cube = getSceneManagerPointer()->getSceneNode("The Cube0");
			Car = getSceneManagerPointer()->getSceneNode("Car");
			auto nodes = getSceneManagerPointer()->getAllNodes();
			auto root = getSceneManagerPointer()->getRootSceneNode();
			auto children = root->getChildrenList();
			std::vector<VESceneNode*>nodeVector{};
				for (auto& n : children) {
					nodeVector.push_back(n);
				}
			children = children[2]->getChildrenList();
				for (auto& n : children) {
					nodeVector.push_back(n);

				}
			//auto list = [children[0]->getName(), children[1]->getName()];
			//+children[3]->getChildrenList();

			//getSceneManagerPointer()->printSceneNodes();

			if (!g_gameLost && !g_win) {
				if (nk_begin(ctx, "", nk_rect(0, 0, 200, 200), NK_WINDOW_BORDER )) {
					char outbuffer[100];

					int a = 100;
					int* len = &a;
					score = g_time * 100;

					nk_layout_row_dynamic(ctx, 45, 1);
					Car->getChildrenList();
					if (nk_button_label(ctx, "RotateLeft")) {
						// event handling
						/*Cube->multiplyTransform(
							glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f, 0.0f, 0.1f)));*/
						

							/*Cube->multiplyTransform(
								glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f, 0.0f, 0.1f)));*/
						Car->setTransform(glm::rotate(Car->getTransform(), -0.4f, glm::vec3(0, 1, 0)));
					}
					if (nk_button_label(ctx, "RotateRight")) {
						/*Cube->multiplyTransform(
							glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 0.0f, -0.1f)));*/
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

				/*	nk_layout_row_dynamic(ctx, 45, 1);
					for (auto& node : nodeVector) {
						if (nk_option_label(ctx, node->getName().c_str(), choice == node->getName())) {
							choice = node->getName();
							dynamic = getSceneManagerPointer()->getSceneNode(choice);
						}
					}*/
					/*if (nk_option_label(ctx, "Scene", choice == "Scene")) {

						choice = "Scene";
						std::cout << choice << std::endl;
						dynamic = getSceneManagerPointer()->getSceneNode(choice);
						pos[0] = dynamic->getPosition()[0];
						pos[1] = dynamic->getPosition()[1];
						pos[2] = dynamic->getPosition()[2];
						sprintf(outbuffer1, "Position: %004.1lf, %004.1lf, %004.1lf", pos[0], pos[1], pos[2]);

					}
					if (nk_option_label(ctx, "Car", choice == "Car")) {
						choice = "Car"; 
						std::cout << choice << std::endl;
						dynamic = getSceneManagerPointer()->getSceneNode(choice);
						pos[0] = dynamic->getPosition()[0];
						pos[1] = dynamic->getPosition()[1];
						pos[2] = dynamic->getPosition()[2];
						sprintf(outbuffer1, "Position: %004.1lf, %004.1lf, %004.1lf", pos[0], pos[1], pos[2]);

					}

					if (nk_option_label(ctx, "Light", choice == "Light")) {
						choice = "Light";



					}
					if (nk_option_label(ctx, "Finish", choice == "Finish")) {
						choice = "Finish";
						std::cout << choice << std::endl;
						dynamic = getSceneManagerPointer()->getSceneNode(choice);
						pos[0] = dynamic->getPosition()[0];
						pos[1] = dynamic->getPosition()[1];
						pos[2] = dynamic->getPosition()[2];
						sprintf(outbuffer1, "Position: %004.1lf, %004.1lf, %004.1lf", pos[0], pos[1], pos[2]);

						

					}*/
					//if (dynamic != nullptr) {
					//	if (choice == "StandardDirLight") {
					//		VELight* light = getSceneManagerPointer()->getLights()[1];

					//		nk_color ambientColor = nk_rgba_f(light->m_col_ambient.r, light->m_col_ambient.g, light->m_col_ambient.b, light->m_col_ambient.a);
					//		nk_color diffuseColor = nk_rgba_f(light->m_col_diffuse.r, light->m_col_diffuse.g, light->m_col_diffuse.b, light->m_col_diffuse.a);
					//		nk_color specularColor = nk_rgba_f(light->m_col_diffuse.r, light->m_col_diffuse.g, light->m_col_diffuse.b, light->m_col_diffuse.a);

					//		//display light name
					//		nk_layout_row_dynamic(ctx, 25, 1);
					//		sprintf(outbuffer, light->getName().c_str());
					//		nk_label(ctx, outbuffer, NK_TEXT_LEFT);

					//		//display light color picker
					//		nk_layout_row_static(ctx, 175, 225, 2);
					//		sprintf(outbuffer, "Ambient:");
					//		nk_label(ctx, outbuffer, NK_TEXT_LEFT);
					//		nk_color newAmbient = nk_color_picker(ctx, ambientColor, NK_RGBA);

					//		nk_layout_row_static(ctx, 175, 225, 2);
					//		sprintf(outbuffer, "Diffuse:");
					//		nk_label(ctx, outbuffer, NK_TEXT_LEFT);
					//		nk_color newDiffuse = nk_color_picker(ctx, diffuseColor, NK_RGBA);

					//		nk_layout_row_static(ctx, 175, 225, 2);
					//		sprintf(outbuffer, "Specular:");
					//		nk_label(ctx, outbuffer, NK_TEXT_LEFT);
					//		nk_color newSpecular = nk_color_picker(ctx, specularColor, NK_RGBA);


					//		//set new light color
					//		light->m_col_ambient = convertTovec4(newAmbient);
					//		light->m_col_diffuse = convertTovec4(newDiffuse);
					//		light->m_col_specular = convertTovec4(newSpecular);
					//		//}

					//	}
					//	else {

					//		nk_layout_row_dynamic(ctx, 45, 1);
					//		double posX = dynamic->getPosition()[0];
					//		double posY = dynamic->getPosition()[1];
					//		double posZ = dynamic->getPosition()[2]; 
					//		sprintf(outbuffer, "X:%004.1lf,Y:%004.1lf,Z: %004.1lf", posX, posY, posZ);
					//		nk_label(ctx, outbuffer, NK_TEXT_LEFT);

					//		nk_text_edit* text{ };
					//		//sprintf(text->string, "Position: %004.1lf, %004.1lf, %004.1lf", pos[0], pos[1], pos[2]);
					//		//nk_textedit_init_default(text);
					//		//if (nk_edit_buffer(ctx, NK_EDIT_SIMPLE, text, nk_filter_ascii))
					//		nk_layout_row_dynamic(ctx, 45, 1);
					//		if (nk_button_label(ctx, "X+")) {
					//			dynamic->setPosition(glm::vec3(posX + 1, posY, posZ));
					//		}
					//		if (nk_button_label(ctx, "X-")) {
					//			dynamic->setPosition(glm::vec3(posX - 1, posY, posZ));
					//		}
					//		if (nk_button_label(ctx, "Z+")) {
					//			dynamic->setPosition(glm::vec3(posX, posY, posZ + 1));
					//		}
					//		if (nk_button_label(ctx, "Z-")) {
					//			dynamic->setPosition(glm::vec3(posX, posY, posZ - 1));
					//		}
					//		nk_layout_row_dynamic(ctx, 45, 1);
					//		auto enu = dynamic->getNodeType();
					//		if (enu == 0) {
					//			sprintf(outbuffer, "Type: %s", "VE_NODE_TYPE_SCENENODE");

					//		}
					//		else {
					//			sprintf(outbuffer, "Type: %s", "VE_NODE_TYPE_SCENEOBJECT");

					//		}
					//		nk_label(ctx, outbuffer, NK_TEXT_LEFT);


					//	}
					//		

					//}
					
					//else {
					//	if (choice == "StandardDirLight") {
					//		VELight* light = getSceneManagerPointer()->getLights()[1];

			
					//		nk_color ambient = nk_rgba_f(light->m_col_ambient.r, light->m_col_ambient.g, light->m_col_ambient.b, light->m_col_ambient.a);
					//		nk_color diffuse = nk_rgba_f(light->m_col_diffuse.r, light->m_col_diffuse.g, light->m_col_diffuse.b, light->m_col_diffuse.a);
					//		nk_color specular = nk_rgba_f(light->m_col_diffuse.r, light->m_col_diffuse.g, light->m_col_diffuse.b, light->m_col_diffuse.a);

					//		//display light name
					//		nk_layout_row_dynamic(ctx, 25, 1);
					//		sprintf(outbuffer, light->getName().c_str());
					//		nk_label(ctx, outbuffer, NK_TEXT_LEFT);

					//		//display light color picker
					//		nk_layout_row_static(ctx, 175, 225, 2);
					//		sprintf(outbuffer, "Ambient:");
					//		nk_label(ctx, outbuffer, NK_TEXT_LEFT);
					//		nk_color newAmbient = nk_color_picker(ctx, ambient, NK_RGBA);

					//		nk_layout_row_static(ctx, 175, 225, 2);
					//		sprintf(outbuffer, "Diffuse:");
					//		nk_label(ctx, outbuffer, NK_TEXT_LEFT);
					//		nk_color newDiffuse = nk_color_picker(ctx, diffuse, NK_RGBA);

					//		nk_layout_row_static(ctx, 175, 225, 2);
					//		sprintf(outbuffer, "Specular:");
					//		nk_label(ctx, outbuffer, NK_TEXT_LEFT);
					//		nk_color newSpecular= nk_color_picker(ctx, specular, NK_RGBA);


					//		//set new light color
					//		light->m_col_ambient = convertTovec4(newAmbient);
					//		light->m_col_diffuse = convertTovec4(newDiffuse);
					//		light->m_col_specular = convertTovec4(newSpecular);
					//		//}
					//	
					//	}


					//}
					


					
				}
			}
			else {
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

			//glm::vec3 positionCube = getSceneManagerPointer()->getSceneNode("Finish")->getPosition();
			//float distance = glm::length(positionCar - positionCube );
			//if (distance < 4) {
			//	//getEnginePointer()->m_irrklangEngine->play2D("media/sounds/explosion.wav", false);
			//	g_win = true;

			//	//VESceneNode *eParent = getSceneManagerPointer()->getSceneNode("The Cube Parent");
			//	//eParent->setPosition(glm::vec3(d(e), 1.0f, d(e)));
			//	/*VESceneNode* pScene = getSceneManagerPointer()->getSceneNode("pScene");

			//	getSceneManagerPointer()->deleteSceneNodeAndChildren("The Cube"+ std::to_string(0));
			//	VECHECKPOINTER(getSceneManagerPointer()->loadModel("The Cube"+ std::to_string(1)  , "media/models/test/crate0", "cube.obj", 0, pScene) );*/
			//}
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

	

	/*class EventListenerCollision : public VEEventListener {
	protected:
		virtual void onFrameStarted(veEvent event) {
			static uint32_t cubeid = 0;

			if (g_restart) {
				g_gameLost = false;
				g_restart = false;
				g_time = 30;
				g_score = 0;
				getSceneManagerPointer()->getSceneNode("The Cube Parent")->setPosition(glm::vec3(d(e), 1.0f, d(e)));
				getEnginePointer()->m_irrklangEngine->play2D("media/sounds/ophelia.wav", true);
				return;
			}
			if (g_gameLost) return;

			glm::vec3 positionCube   = getSceneManagerPointer()->getSceneNode("The Cube Parent")->getPosition();
			glm::vec3 positionCamera = getSceneManagerPointer()->getSceneNode("StandardCameraParent")->getPosition();

			float distance = glm::length(positionCube - positionCamera);
			if (distance < 1) {
				g_score++;
				getEnginePointer()->m_irrklangEngine->play2D("media/sounds/explosion.wav", false);
				if (g_score % 10 == 0) {
					g_time = 30;
					getEnginePointer()->m_irrklangEngine->play2D("media/sounds/bell.wav", false);
				}

				VESceneNode *eParent = getSceneManagerPointer()->getSceneNode("The Cube Parent");
				eParent->setPosition(glm::vec3(d(e), 1.0f, d(e)));

				getSceneManagerPointer()->deleteSceneNodeAndChildren("The Cube"+ std::to_string(cubeid));
				VECHECKPOINTER(getSceneManagerPointer()->loadModel("The Cube"+ std::to_string(++cubeid)  , "media/models/test/crate0", "cube.obj", 0, eParent) );
			}

			g_time -= event.dt;
			if (g_time <= 0) {
				g_gameLost = true;
				getEnginePointer()->m_irrklangEngine->removeAllSoundSources();
				getEnginePointer()->m_irrklangEngine->play2D("media/sounds/gameover.wav", false);
			}
		};

	public:
		///Constructor of class EventListenerCollision
		EventListenerCollision(std::string name) : VEEventListener(name) { };

		///Destructor of class EventListenerCollision
		virtual ~EventListenerCollision() {};
	};*/



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
			//eParent = getSceneManagerPointer()->createSceneNode("The Cube Parent", pScene, glm::mat4(1.0));
			//VECHECKPOINTER(e1 = getSceneManagerPointer()->loadModel("The Cube0", "media/models/test/crate2", "cube.obj"));
			//VECHECKPOINTER(e1 = getSceneManagerPointer()->loadModel("The Cube0", "media/models/test/crate2", "cube.obj", 0, pScene));
			//e1->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 15.0f)));
			//eParent->addChild(e1);
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

			//VESceneNode* road1;
			//VECHECKPOINTER(road1 = getSceneManagerPointer()->loadModel("turn1", "media/models/test/myRoad", "myRoad.obj", 0, pScene));
			//road1->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.6f, 0.6f)));
			//road1->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.5f, -4.0f)));
			//road1->setTransform(glm::rotate(road1->getTransform(), 1.5707f, glm::vec3(0, 1, 0)));
			//m_irrklangEngine->play2D("media/sounds/ophelia.wav", true);
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
