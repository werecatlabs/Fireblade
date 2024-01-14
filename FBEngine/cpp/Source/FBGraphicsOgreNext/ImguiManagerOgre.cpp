#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include "FBGraphicsOgreNext/ImguiManagerOgre.h"

#if FB_GRAPHICS_SYSTEM_OGRENEXT

#include <OgrePass.h>
#include <OgreSceneManager.h>
#include <OgreHighLevelGpuProgramManager.h>
#include <OgreUnifiedHighLevelGpuProgram.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgreHlmsDatablock.h>
//#include <Deprecated/OgreTextureManager.h>
//#include <Deprecated/OgreHardwarePixelBuffer.h>
#include <OgreViewport.h>
#include <OgreRenderSystem.h>
#include <OgreRoot.h>
#include "FBImGui/ImguiOgre/FontCousine_Regular.h"

#include <CommandBuffer/OgreCbDrawCall.h>

#include <OgrePsoCacheHelper.h>
#include "imgui.h"

//#include "System/Setup/SystemSettings.h"


#if FB_BUILD_SDL2
#include <SDL.h>
#endif

//#ifdef _WIN32
//#include <OgreD3D11RenderSystem.h>
//#include <OgreD3D11Device.h>
//#endif

template <>
ImguiManagerOgre *Ogre::Singleton<ImguiManagerOgre>::msSingleton = nullptr;

auto ImguiManagerOgre::getSingletonPtr() -> ImguiManagerOgre *
{
	if (!msSingleton)
	{
		msSingleton = new ImguiManagerOgre();
	}
	return msSingleton;
}
auto ImguiManagerOgre::getSingleton() -> ImguiManagerOgre &
{
	if (!msSingleton)
	{
		msSingleton = new ImguiManagerOgre();
	}
	return (*msSingleton);
}

ImguiManagerOgre::ImguiManagerOgre() : mSceneMgr( nullptr ), mLastRenderedFrame( 4 ), mFrameEnded( true )
{
}
ImguiManagerOgre::~ImguiManagerOgre()
{
	//delete mPSOCache;
}

void ImguiManagerOgre::shutdown() {
	while (mRenderables.size() > 0)
	{
		delete mRenderables.back();
		mRenderables.pop_back();
	}

	//Ogre::TextureManager::getSingleton().remove(mFontTex);
	delete mPSOCache;

	//std::string name = mSceneMgr->getDestinationRenderSystem()->getName();
	//if (name == "Direct3D11 Rendering Subsystem") {
	//    //This seems to me to be a work around.
	//    //For some reason I was seeing assert failures when trying to destroy manual materials, OgreD3D11DeviceResource.cpp, line 59, it was checking if the device resources were empty.
	//    //There was however the manual texture left over.
	//    //This was the way I found to get rid of it.
	//    mFontTex->~Texture();
	//}
}

void ImguiManagerOgre::init(Ogre::SceneManager* mgr)
{
	mSceneMgr = mgr;

	mPSOCache = new Ogre::PsoCacheHelper(Ogre::Root::getSingletonPtr()->getRenderSystem());

	createFontTexture();
	createMaterial();
}

void ImguiManagerOgre::newFrame()
{
	mFrameEnded = false;
	ImGuiIO& io = ImGui::GetIO();

	//static Uint64 g_Time = 0;
	//static Uint64 frequency = SDL_GetPerformanceFrequency();
	//Uint64 current_time = SDL_GetPerformanceCounter();
	//io.DeltaTime = g_Time > 0 ? (float)((double)(current_time - g_Time) / frequency) : (float)(1.0f / 60.0f);
	//g_Time = current_time;

	//Ogre::Viewport *vp = mSceneMgr->getCurrentViewport();

	//just some defaults so it doesn't crash
	float width = 400.f;
	float height = 400.f;

	//might not exist if this got called before the rendering loop
	//if (vp)
	//{
	//	width = mSceneMgr->getCurrentViewport()->getActualWidth();
	//	height = mSceneMgr->getCurrentViewport()->getActualHeight();
	//}

	// Setup display size (every frame to accommodate for window resizing)
	io.DisplaySize = ImVec2(width, height);

	// Start the frame
	ImGui::NewFrame();

	 bool show_demo_window;
	 ImGui::ShowDemoWindow(&show_demo_window);
}

void ImguiManagerOgre::updateProjectionMatrix(float width, float height) {
	Ogre::Matrix4 projMatrix(2.0f / width, 0.0f, 0.0f, -1.0f,
		0.0f, -2.0f / height, 0.0f, 1.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	mPass->getVertexProgramParameters()->setNamedConstant("ProjectionMatrix", projMatrix);

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(width, height);
}

void ImguiManagerOgre::render()
{
	//#ifdef __APPLE__
	//    @autoreleasepool{
	//#endif

		//Cancel rendering if not necessary
		//or if newFrame didn't got called
    if( mFrameEnded )
    {
        return;
    }
    mFrameEnded = true;

	int currentFrame = ImGui::GetFrameCount();
	if (currentFrame == mLastRenderedFrame)
	{
		return;
	}
	mLastRenderedFrame = currentFrame;

	//Tell ImGui to create the buffers
	ImGui::Render();

	//Needed later
	//vp = mSceneMgr->getCurrentViewport();

	ImGuiIO& io = ImGui::GetIO();
	if (currentFrame < 5) {
		//Update the matrix for the first few frames, to make sure everything's synchronised.
		updateProjectionMatrix(io.DisplaySize.x, io.DisplaySize.y);
	}
	if (currentFrame <= 1) {
		//Lots of stuff can be done only once for the sake of efficiency.


		mPSOCache->clearState();
		//mPSOCache->setRenderTarget(vp->getTarget());   // dark_sylinc's advice on setting rendertarget, which looks like the renderwindow obj)

		const Ogre::HlmsBlendblock* blendblock = mPass->getBlendblock();
		const Ogre::HlmsMacroblock* macroblock = mPass->getMacroblock();
		mPSOCache->setMacroblock(macroblock);
		mPSOCache->setBlendblock(blendblock);
		mPSOCache->setVertexShader(const_cast<Ogre::GpuProgramPtr&>(mPass->getVertexProgram()));
		mPSOCache->setPixelShader(const_cast<Ogre::GpuProgramPtr&>(mPass->getFragmentProgram()));
	}


	ImDrawData* drawData = ImGui::GetDrawData();
	int numberDraws = 0;

	//iterate through all lists (at the moment every window has its own)
	for (int n = 0; n < drawData->CmdListsCount; n++)
	{
		const ImDrawList* drawList = drawData->CmdLists[n];
		const ImDrawVert* vtxBuf = drawList->VtxBuffer.Data;
		const ImDrawIdx* idxBuf = drawList->IdxBuffer.Data;

		unsigned int startIdx = 0;

		for (int i = 0; i < drawList->CmdBuffer.Size; i++)
		{
			//create renderables if necessary
			//This creates a new one each time it notices the list is bigger than necessary.
			if (i >= mRenderables.size())
			{
				mRenderables.push_back(new ImguiRenderable());
			}

			//update their vertex buffers
			const ImDrawCmd* drawCmd = &drawList->CmdBuffer[i];
			mRenderables[i]->updateVertexData(vtxBuf, &idxBuf[startIdx], drawList->VtxBuffer.Size, drawCmd->ElemCount);

			//set scissoring
			int vpLeft, vpTop, vpWidth, vpHeight;
			//vp->getActualDimensions(vpLeft, vpTop, vpWidth, vpHeight);

			int scLeft = drawCmd->ClipRect.x;
			int scTop = drawCmd->ClipRect.y;
			int scRight = drawCmd->ClipRect.z;
			int scBottom = drawCmd->ClipRect.w;

			scLeft = scLeft < 0 ? 0 : (scLeft > vpWidth ? vpWidth : scLeft);
			scRight = scRight < 0 ? 0 : (scRight > vpWidth ? vpWidth : scRight);
			scTop = scTop < 0 ? 0 : (scTop > vpHeight ? vpHeight : scTop);
			scBottom = scBottom < 0 ? 0 : (scBottom > vpHeight ? vpHeight : scBottom);

            float left = static_cast<float>( scLeft ) / static_cast<float>( vpWidth );
            float top = static_cast<float>( scTop ) / static_cast<float>( vpHeight );
            float width = static_cast<float>( scRight - scLeft ) / static_cast<float>( vpWidth );
            float height = static_cast<float>( scBottom - scTop ) / static_cast<float>( vpHeight );

            //vp->setScissors(left, top, width, height);
			//mSceneMgr->getDestinationRenderSystem()->_setViewport(vp);

			//render the object
			//mSceneMgr->_injectRenderWithPass(mPass, mRenderables[i], 0, false, false);


			Ogre::v1::RenderOperation renderOp;
			mRenderables[i]->getRenderOperation(renderOp, false);

			bool enablePrimitiveRestart = true;  // tried both true and false...no change

			Ogre::VertexElement2VecVec vertexElements = renderOp.vertexData->
				vertexDeclaration->convertToV2();
			mPSOCache->setVertexFormat(vertexElements,
				renderOp.operationType,
				enablePrimitiveRestart);

			Ogre::HlmsPso* pso = mPSOCache->getPso();
			mSceneMgr->getDestinationRenderSystem()->_setPipelineStateObject(pso);

			mSceneMgr->getDestinationRenderSystem()->bindGpuProgramParameters(Ogre::GPT_VERTEX_PROGRAM, mPass->getVertexProgramParameters(), Ogre::GPV_ALL);

			//mSceneMgr->getDestinationRenderSystem()->_setTexture(0, true, (Ogre::Texture*)drawCmd->TextureId);

			Ogre::v1::CbRenderOp op(renderOp);
			mSceneMgr->getDestinationRenderSystem()->_setRenderOperation(&op);

			mSceneMgr->getDestinationRenderSystem()->_render(renderOp);


			//increase start index of indexbuffer
			startIdx += drawCmd->ElemCount;
			numberDraws++;
		}
	}

	//reset Scissors
	vp->setScissors(0, 0, 1, 1);
	//mSceneMgr->getDestinationRenderSystem()->_setViewport(vp);

	//delete unused renderables
	while (mRenderables.size() > numberDraws)
	{
		delete mRenderables.back();
		mRenderables.pop_back();
	}
	//#ifdef __APPLE__
	//    }
	//#endif
}

void ImguiManagerOgre::createMaterial()
{
	static const char* vertexShaderSrcD3D11 =
	{
		"uniform float4x4 ProjectionMatrix;\n"
		"struct VS_INPUT\n"
		"{\n"
		"float2 pos : POSITION;\n"
		"float4 col : COLOR0;\n"
		"float2 uv  : TEXCOORD0;\n"
		"};\n"
		"struct PS_INPUT\n"
		"{\n"
		"float4 pos : SV_POSITION;\n"
		"float4 col : COLOR0;\n"
		"float2 uv  : TEXCOORD0;\n"
		"};\n"
		"PS_INPUT main(VS_INPUT input)\n"
		"{\n"
		"PS_INPUT output;\n"
		"output.pos = mul(ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\n"
		"output.col = input.col;\n"
		"output.uv  = input.uv;\n"
		"return output;\n"
		"}"
	};
	static const char* pixelShaderSrcD3D11 =
	{
		"struct PS_INPUT\n"
		"{\n"
		"float4 pos : SV_POSITION;\n"
		"float4 col : COLOR0;\n"
		"float2 uv  : TEXCOORD0;\n"
		"};\n"
		"sampler sampler0: register(s0);\n"
		"Texture2D texture0: register(t0);\n"
		"\n"
		"float4 main(PS_INPUT input) : SV_Target\n"
		"{\n"
		"float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \n"
		"return out_col; \n"
		"}"
	};


	static const char* vertexShaderSrcGLSL =
	{
		"#version 150\n"
		"uniform mat4 ProjectionMatrix; \n"
		"in vec2 vertex;\n"
		"in vec2 uv0;\n"
		"in vec4 colour;\n"
		"out vec2 Texcoord;\n"
		"out vec4 col;\n"
		"void main()\n"
		"{\n"
		"gl_Position = ProjectionMatrix* vec4(vertex.xy, 0.f, 1.f);\n"
		"Texcoord  = uv0;\n"
		"col = colour;\n"
		"}"
	};
	static const char* pixelShaderSrcGLSL =
	{
		"#version 150\n"
		"in vec2 Texcoord;\n"
		"in vec4 col;\n"
		"uniform sampler2D sampler0;\n"
		"out vec4 out_col;\n"
		"void main()\n"
		"{\n"
		"out_col = col * texture(sampler0, Texcoord);\n"
		"}"
	};


	static const char* fragmentShaderSrcMetal =
	{
		"#include <metal_stdlib>\n"
		"using namespace metal;\n"
		"\n"
		"struct VertexOut {\n"
		"    float4 position [[position]];\n"
		"    float2 texCoords;\n"
		"    float4 colour;\n"
		"};\n"
		"\n"
		"fragment float4 main_metal(VertexOut in [[stage_in]],\n"
		"                             texture2d<float> texture [[texture(0)]]) {\n"
		"    constexpr sampler linearSampler(coord::normalized, min_filter::linear, mag_filter::linear, mip_filter::linear);\n"
		"    float4 texColour = texture.sample(linearSampler, in.texCoords);\n"
		"    return in.colour * texColour;\n"
		"}\n"
	};

	static const char* vertexShaderSrcMetal =
	{
		"#include <metal_stdlib>\n"
		"using namespace metal;\n"
		"\n"
		"struct Constant {\n"
		"    float4x4 ProjectionMatrix;\n"
		"};\n"
		"\n"
		"struct VertexIn {\n"
		"    float2 position  [[attribute(VES_POSITION)]];\n"
		"    float2 texCoords [[attribute(VES_TEXTURE_COORDINATES0)]];\n"
		"    float4 colour     [[attribute(VES_DIFFUSE)]];\n"
		"};\n"
		"\n"
		"struct VertexOut {\n"
		"    float4 position [[position]];\n"
		"    float2 texCoords;\n"
		"    float4 colour;\n"
		"};\n"
		"\n"
		"vertex VertexOut vertex_main(VertexIn in                 [[stage_in]],\n"
		"                             constant Constant &uniforms [[buffer(PARAMETER_SLOT)]]) {\n"
		"    VertexOut out;\n"
		"    out.position = uniforms.ProjectionMatrix * float4(in.position, 0, 1);\n"

		"    out.texCoords = in.texCoords;\n"
		"    out.colour = in.colour;\n"

		"    return out;\n"
		"}\n"
	};


	//create the default shadows material
	Ogre::HighLevelGpuProgramManager& mgr = Ogre::HighLevelGpuProgramManager::getSingleton();

	Ogre::HighLevelGpuProgramPtr vertexShaderUnified = mgr.getByName("imgui/VP");
	Ogre::HighLevelGpuProgramPtr pixelShaderUnified = mgr.getByName("imgui/FP");

	Ogre::HighLevelGpuProgramPtr vertexShaderD3D11 = mgr.getByName("imgui/VP/D3D11");
	Ogre::HighLevelGpuProgramPtr pixelShaderD3D11 = mgr.getByName("imgui/FP/D3D11");

	Ogre::HighLevelGpuProgramPtr vertexShaderGL = mgr.getByName("imgui/VP/GL150");
	Ogre::HighLevelGpuProgramPtr pixelShaderGL = mgr.getByName("imgui/FP/GL150");

	Ogre::HighLevelGpuProgramPtr vertexShaderMetal = mgr.getByName("imgui/VP/Metal");
	Ogre::HighLevelGpuProgramPtr pixelShaderMetal = mgr.getByName("imgui/FP/Metal");

	if (vertexShaderUnified.isNull())
	{
		vertexShaderUnified = mgr.createProgram("imgui/VP", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "unified", Ogre::GPT_VERTEX_PROGRAM);
	}
	if (pixelShaderUnified.isNull())
	{
		pixelShaderUnified = mgr.createProgram("imgui/FP", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "unified", Ogre::GPT_FRAGMENT_PROGRAM);
	}

    auto *vertexShaderPtr = static_cast<Ogre::UnifiedHighLevelGpuProgram *>( vertexShaderUnified.get() );
    auto *pixelShaderPtr = static_cast<Ogre::UnifiedHighLevelGpuProgram *>( pixelShaderUnified.get() );

    if (vertexShaderD3D11.isNull())
	{
		vertexShaderD3D11 = mgr.createProgram("imgui/VP/D3D11", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			"hlsl", Ogre::GPT_VERTEX_PROGRAM);
		vertexShaderD3D11->setParameter("target", "vs_4_0");
		vertexShaderD3D11->setParameter("entry_point", "main");
		vertexShaderD3D11->setSource(vertexShaderSrcD3D11);
		vertexShaderD3D11->load();

		vertexShaderPtr->addDelegateProgram(vertexShaderD3D11->getName());
	}
	if (pixelShaderD3D11.isNull())
	{
		pixelShaderD3D11 = mgr.createProgram("imgui/FP/D3D11", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			"hlsl", Ogre::GPT_FRAGMENT_PROGRAM);
		pixelShaderD3D11->setParameter("target", "ps_4_0");
		pixelShaderD3D11->setParameter("entry_point", "main");
		pixelShaderD3D11->setSource(pixelShaderSrcD3D11);
		pixelShaderD3D11->load();

		pixelShaderPtr->addDelegateProgram(pixelShaderD3D11->getName());
	}

	if (vertexShaderMetal.isNull()) {
		vertexShaderMetal = mgr.createProgram("imgui/VP/Metal", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			"metal", Ogre::GPT_VERTEX_PROGRAM);
		vertexShaderMetal->setParameter("entry_point", "vertex_main");
		vertexShaderMetal->setSource(vertexShaderSrcMetal);
		vertexShaderMetal->load();
		vertexShaderPtr->addDelegateProgram(vertexShaderMetal->getName());

	}
	if (pixelShaderMetal.isNull()) {
		pixelShaderMetal = mgr.createProgram("imgui/FP/Metal", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			"metal", Ogre::GPT_FRAGMENT_PROGRAM);
		vertexShaderMetal->setParameter("entry_point", "fragment_main");
		pixelShaderMetal->setSource(fragmentShaderSrcMetal);
		pixelShaderMetal->load();
		pixelShaderPtr->addDelegateProgram(pixelShaderMetal->getName());
	}


	if (vertexShaderGL.isNull())
	{
		vertexShaderGL = mgr.createProgram("imgui/VP/GL150", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			"glsl", Ogre::GPT_VERTEX_PROGRAM);
		vertexShaderGL->setSource(vertexShaderSrcGLSL);
		vertexShaderGL->load();
		vertexShaderPtr->addDelegateProgram(vertexShaderGL->getName());
	}
	if (pixelShaderGL.isNull())
	{
		pixelShaderGL = mgr.createProgram("imgui/FP/GL150", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			"glsl", Ogre::GPT_FRAGMENT_PROGRAM);
		pixelShaderGL->setSource(pixelShaderSrcGLSL);
		pixelShaderGL->load();
		pixelShaderGL->setParameter("sampler0", "int 0");

		pixelShaderPtr->addDelegateProgram(pixelShaderGL->getName());
	}


	Ogre::MaterialPtr imguiMaterial = Ogre::MaterialManager::getSingleton().create("imgui/material", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mPass = imguiMaterial->getTechnique(0)->getPass(0);
	mPass->setFragmentProgram("imgui/FP");
	mPass->setVertexProgram("imgui/VP");


	Ogre::HlmsBlendblock blendblock(*mPass->getBlendblock());
	blendblock.mSourceBlendFactor = Ogre::SBF_SOURCE_ALPHA;
	blendblock.mDestBlendFactor = Ogre::SBF_ONE_MINUS_SOURCE_ALPHA;
	blendblock.mSourceBlendFactorAlpha = Ogre::SBF_ONE_MINUS_SOURCE_ALPHA;
	blendblock.mDestBlendFactorAlpha = Ogre::SBF_ZERO;
	blendblock.mBlendOperation = Ogre::SBO_ADD;
	blendblock.mBlendOperationAlpha = Ogre::SBO_ADD;
	blendblock.mSeparateBlend = true;
	blendblock.mIsTransparent = true;

	Ogre::HlmsMacroblock macroblock(*mPass->getMacroblock());
	macroblock.mCullMode = Ogre::CULL_NONE;
	macroblock.mDepthFunc = Ogre::CMPF_ALWAYS_PASS;
	macroblock.mDepthCheck = false;
	macroblock.mDepthWrite = false;
	macroblock.mScissorTestEnabled = true;

	mPass->setBlendblock(blendblock);
	mPass->setMacroblock(macroblock);

	Ogre::String renderSystemName = mSceneMgr->getDestinationRenderSystem()->getName();
	//Apparently opengl was the only one that needed this.
	if (renderSystemName == "OpenGL 3+ Rendering Subsystem") {
		mPass->getFragmentProgramParameters()->setNamedConstant("sampler0", 0);
	}
	mPass->createTextureUnitState()->setTextureName("ImguiFontTex");
}

void ImguiManagerOgre::createFontTexture()
{
	// Build texture atlas
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;

	//Use the default font if there is no content scaling because it looks better.
	float guiScale = 1;
	if(guiScale >= 2.0f){
		//13 being the default font size.
        ImFont *font = io.Fonts->AddFontFromMemoryCompressedBase85TTF(
            MyFont_compressed_data_base85, static_cast<int>( guiScale * 13 ) );
#ifdef __APPLE__
            //This is a work around for macOS.
            //Currently the only way I've found to represent high-res fonts with the metal renderer is to do this.
            //My theory is that this is something to do with my metal imgui implementation, but I haven't found the cause yet.
            font->Scale = guiScale;
        #endif
	}else{
		io.Fonts->AddFontDefault();
	}
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	//mFontTex = Ogre::TextureManager::getSingleton().createManual("ImguiFontTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, width, height, 1, 1, Ogre::PF_R8G8B8A8);

 //   io.Fonts->SetTexID(mFontTex.get());

	//const Ogre::PixelBox & lockBox = mFontTex->getBuffer()->lock(Ogre::Image::Box(0, 0, width, height), Ogre::v1::HardwareBuffer::HBL_DISCARD);
	//size_t texDepth = Ogre::PixelUtil::getNumElemBytes(lockBox.format);

	//memcpy(lockBox.data, pixels, width*height*texDepth);
	//mFontTex->getBuffer()->unlock();
}


#endif

