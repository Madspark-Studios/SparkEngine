#pragma once

//TO BE INCLUDED BY CLIENT APPLICATIONS
#include "Spark/Application.h"
#include "Spark/Log.h"

//----ENTRY POINT---------------
#include "Spark/EntryPoint.h"
//------------------------------

//Core
#include "Spark/Events/ApplicationEvent.h"
#include "Spark/Layer.h"
#include "Spark/imgui/ImGUILayer.h"
#include "imgui.h"
#include "Spark/Input.h"
#include "Spark/Core/Timestep.h"

//Rendering
#include "Spark/Renderer/Renderer.h"
#include "Spark/Renderer/Buffer.h"
#include "Spark/Renderer/VertexArray.h"
#include "Spark/Renderer/Shader.h"
#include "Spark/Renderer/Camera.h"

//Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>