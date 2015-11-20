#include "HelloWorldScene.h"
#include "Perlin.h"

USING_NS_CC;



Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	

	auto center = Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

	auto bg = Sprite::create("bg.png");
	bg->setPosition(center);
	auto bgsize = bg->getContentSize();
	float scaleX = visibleSize.width / bgsize.width;
	float scaleY = visibleSize.height / bgsize.height;
	bg->setScaleX(scaleX);
	bg->setScaleY(scaleY);
	addChild(bg);

	auto grass = Sprite::create("grass.png");
	grass->setAnchorPoint(ccp(0.5, 0.5));
	grass->setPosition(center);
	addChild(grass);

	Texture2D::TexParams p;
	p.minFilter = GL_LINEAR;
	p.magFilter = GL_LINEAR;
	p.wrapS = GL_REPEAT;
	p.wrapT = GL_REPEAT;

	glActiveTexture(GL_TEXTURE0);

	auto textureCache = Director::getInstance()->getTextureCache();
	auto tex1 = textureCache->addImage("grass.png");
	//tex1->setTexParameters(p);

	GLProgram* prog = new GLProgram();
	prog->initWithFilenames("perlin_wind.vert", "perlin_wind.frag");
	prog->retain();
	prog->link();
	prog->use();
	prog->updateUniforms();


	float gtime = Director::getInstance()->getDeltaTime();
	float ctime = Director::getInstance()->getTotalFrames() * Director::getInstance()->getAnimationInterval();

	//CCLog("Wind is %f", pnoise);

	ShaderCache::getInstance()->addGLProgram(prog, "perlin_wind");
	
	grass->setGLProgram(prog);
	
	GLProgramState* state = GLProgramState::getOrCreateWithGLProgram(prog);
	
	//state->setUniformFloat("u_gtime", 1.0f);
	//state->setUniformFloat("u_ctime",0.5f);
	//state->setUniformFloat("u_color", m_cloudAmount);
	state->setUniformFloat("u_wind", 0);
	schedule(schedule_selector(HelloWorld::setWind));
	prog->release();

	schedule(schedule_selector(HelloWorld::setWind));
	return true;

	
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::setWind(float dt)
{
	Perlin perlin;
	float pwind = perlin.noise(-0.5f, 0.5f, 0);
	CCLOG("Wind is %f",pwind);

	auto my_prog = ShaderCache::getInstance()->getGLProgram("perlin_wind");
	auto my_state = GLProgramState::getOrCreateWithGLProgram(my_prog);
	
	my_state->setUniformFloat("u_wind", pwind);

}
