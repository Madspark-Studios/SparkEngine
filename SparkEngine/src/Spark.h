#pragma once

//TO BE INCLUDED BY CLIENT APPLICATIONS
#include "Spark/Application.h"
#include "Spark/Log.h"

//----ENTRY POINT---------------
#include "Spark/EntryPoint.h"
//------------------------------

#include "Spark/Events/ApplicationEvent.h"
#include "Spark/Layer.h"
#include "Spark/imgui/ImGUILayer.h"
#include "Spark/Input.h"

//Rendering
#include "Spark/Renderer/Renderer.h"
#include "Spark/Renderer/Buffer.h"
#include "Spark/Renderer/VertexArray.h"
#include "Spark/Platform/OpenGL/OpenGLShader.h"