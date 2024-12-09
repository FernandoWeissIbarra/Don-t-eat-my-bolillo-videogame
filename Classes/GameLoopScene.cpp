#include "GameLoopScene.h"
#include "ui/CocosGUI.h"
#include "Bolillo.h"
#include "Pato.h"
#include "Bullet.h"

USING_NS_CC;

Scene* GameLoopScene::createScene() {
    return GameLoopScene::create();
}

bool GameLoopScene::init() {
    // Ensure the parent Scene initializes correctly
    if (!Scene::init()) {
        return false;
    }

    // Get window size for positioning elements
    Size winSize = Director::getInstance()->getWinSize();

    // Initialize EXP points
    EXP = 0;

    // Create and position the EXP label
    _expLabel = Label::createWithTTF("EXP: 0", "fonts/Marker Felt.ttf", 32);
    _expLabel->setPosition(Vec2(200, 1000));  // Adjust position as needed
    _expLabel->setTextColor(Color4B::WHITE);
    this->addChild(_expLabel, 5);  // Ensure the label is on a visible layer

    // Create and position the background
    Sprite* background = Sprite::create("Background.png");
    background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    // Uncomment the following line to scale the background if needed
    // background->setScale(3.5f);
    this->addChild(background, -1);

    // Create the pause button
    auto pauseButton = ui::Button::create("PauseButtonN.png", "PauseButtonP.png");
    pauseButton->setPosition(Vec2(1600, 1000));
    pauseButton->addClickEventListener([](Ref* sender) {
        // Transition to the pause scene
        auto pauseScene = PauseScene::create();
        Director::getInstance()->pushScene(TransitionFade::create(0.5, pauseScene));
        });
    this->addChild(pauseButton, 5);

    // Create the Bolillo character
    _bolillo = Bolillo::create();
    if (_bolillo) {
        this->addChild(_bolillo, 1);  // Ensure Bolillo is on a layer above the background
        _bolillo->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    }

    // Add initial ducks (Patos) to the scene
    addPatos(20);

    // Schedule the update method
    scheduleUpdate();

    return true;
}

// Update method executed every frame
void GameLoopScene::update(float dt) {
    patoColision();               // Check for collisions with ducks (Patos)
    checkPatoBulletCollisions();  // Check if bullets hit any ducks
    gabiotaColision();            // Check for collisions with seagulls (Gabiotas)
    checkGabiotaBulletCollisions(); // Check if bullets hit any seagulls
    cuervoColision();             // Check for collisions with crows (Cuervos)
    checkCuervoBulletCollisions(); // Check if bullets hit any crows
    palomaColision();             // Check for collisions with pigeons (Palomas)
    checkPalomaBulletCollisions(); // Check if bullets hit any pigeons
    buitreColision();             // Check for collisions with vultures (Buitres)
    checkBuitreBulletCollisions(); // Check if bullets hit any vultures
}

// Adds a specified number of ducks (Patos) to the scene
void GameLoopScene::addPatos(int number) {
    for (int i = 0; i < number; ++i) {
        // Generate a random spawn time between 0 and 20 seconds
        float randomTime = CCRANDOM_0_1() * 20.0f;

        // Schedule each duck's spawn
        this->scheduleOnce([this, i](float) {
            Pato* pato = Pato::create();
            if (pato) {
                patoFlag = true;
                this->addChild(pato, 1);  // Add to the scene
                _patos.pushBack(pato);   // Add to the ducks list
                pato->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
            }
            }, randomTime, "addPato_" + std::to_string(i));
    }
}

// Handles collisions between Bolillo and ducks (Patos)
void GameLoopScene::patoColision() {
    static float tiempoDesdeUltimoDaño = 0.0f; // Time since last damage
    const float intervaloDaño = 0.5f;         // Minimum time between damage

    // Accumulate time since the last frame
    tiempoDesdeUltimoDaño += Director::getInstance()->getDeltaTime();

    if (_bolillo) {
        for (auto pato : _patos) {
            // Pass Bolillo's position to each duck
            pato->setTargetPosition(_bolillo->getPosition());

            // Detect collision
            if (pato->getBoundingBox().intersectsRect(_bolillo->getBoundingBox())) {
                // Apply damage only if the interval has passed
                if (tiempoDesdeUltimoDaño >= intervaloDaño) {
                    _bolillo->reduceLife(3);  // Reduce Bolillo's life
                    tiempoDesdeUltimoDaño = 0.0f;  // Reset damage timer

                    // Check if Bolillo is out of health
                    if (_bolillo->getLife() <= 0) {
                        this->removeChild(_bolillo);
                        _bolillo = nullptr;
                        goToGameOverScene(false);  // Transition to the Game Over scene
                        return;  // Exit to avoid accessing a null object
                    }
                }

                // Exit loop after detecting a collision
                break;
            }
        }
    }
}

// Check for collisions between bullets and ducks (Patos)
void GameLoopScene::checkPatoBulletCollisions() {
    Vector<Bullet*> bulletsToRemove; // Store bullets to be removed

    for (auto bullet : _bullets) {
        for (auto pato : _patos) {
            if (bullet->getBoundingBox().intersectsRect(pato->getBoundingBox())) {
                pato->reduceLife(1); // Reduce duck's health

                if (pato->getLife() <= 0) {
                    if (pato->getScale() > 1.0f && patoBoss) {
                        EXP += 50; // Award extra EXP for boss
                        goToStoreScene(); // Transition to store scene
                        addGabiotas(20); // Add seagulls (Gabiotas)
                    }
                    cocos2d::AudioEngine::play2d("cua.mp3", false);
                    this->removeChild(pato);
                    _patos.eraseObject(pato); // Remove duck from the list
                    EXP += 5; // Award EXP
                    _expLabel->setString("EXP: " + std::to_string(EXP));
                }
                bulletsToRemove.pushBack(bullet); // Mark bullet for removal
                break;
            }
        }
    }

    // Remove marked bullets
    for (auto bullet : bulletsToRemove) {
        this->removeChild(bullet);
        _bullets.eraseObject(bullet);
    }

    // If all ducks are defeated and no boss exists, spawn a boss duck
    if (_patos.empty() && patoFlag && !patoBoss) {
        Pato* bossPato = Pato::create();
        if (bossPato) {
            bossPato->BossMode(); // Enable boss mode
            this->addChild(bossPato, 1);
            _patos.pushBack(bossPato);
            patoBoss = true;
        }
    }
}
void GameLoopScene::addGabiotas(int number) {

    addPatos(5);

    for (int i = 0; i < number; ++i) {
        // Generar un tiempwo aleatorio entre 0 y 10 segundos
        float randomTime = CCRANDOM_0_1() * 10.0f;

        // Usar un scheduler para llamar una función con un retraso aleatorio
        this->scheduleOnce([this, i](float) {
            Gabiota* gabiota = Gabiota::create();
            if (gabiota) {
                gabiotaFlag = true;
                this->addChild(gabiota, 1);
                _gabiotas.pushBack(gabiota); // Agregar al vector
                gabiota->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
            }
            }, randomTime, "addGabiota_" + std::to_string(i));
    }
}

void GameLoopScene::gabiotaColision() {
    static float tiempoDesdeUltimoDaño = 0.0f; 
    const float intervaloDaño = 0.5f;        

    tiempoDesdeUltimoDaño += Director::getInstance()->getDeltaTime();

    if (_bolillo) {
        for (auto gabiota : _gabiotas) {
            gabiota->setTargetPosition(_bolillo->getPosition());

            // Detectar colisión
            if (gabiota->getBoundingBox().intersectsRect(_bolillo->getBoundingBox())) {
                // Verificar si el intervalo de tiempo ha pasado
                if (tiempoDesdeUltimoDaño >= intervaloDaño) {
                    // Reducir la vida de _bolillo
                    _bolillo->reduceLife(1);
                    tiempoDesdeUltimoDaño = 0.0f; // Reinicia el tiempo del temporizador

                    // Verificar si Bolillo debe desaparecer
                    if (_bolillo->getLife() <= 0) {
                        this->removeChild(_bolillo);
                        _bolillo = nullptr;
                        goToGameOverScene(false);
                        return; // Salir para evitar accesos inválidos
                    }
                }

                // Si ya se detectó colisión y aplicó daño, no es necesario seguir revisando más
                break;
            }
        }
    }
}

void GameLoopScene::checkGabiotaBulletCollisions() {
    // Crear una lista temporal para almacenar balas que deben eliminarse
    Vector<Bullet*> bulletsToRemove;

    for (auto bullet : _bullets) {
        for (auto gabiota : _gabiotas) {
            if (bullet->getBoundingBox().intersectsRect(gabiota->getBoundingBox())) {
                // Reducir vida del gabiota
                gabiota->reduceLife(1);

                // Eliminar el gabiota si su vida llega a 0
                if (gabiota->getLife() <= 0) {
                    // Eliminar el gabiota de la escena y de la lista
                    if (gabiota->getScale() > 1.0f && gabiotaBoss) { // Asumimos que el jefe tiene un tamaño mayor
                        // Activar el spawn de cuervos al morir el bossGabiota
                        EXP += 60;
                        goToStoreScene();
                        addCuervos(8); // Ajusta el número de gaviotas que deseas agregar
                    }
                    cocos2d::AudioEngine::play2d("cua.mp3", false);
                    this->removeChild(gabiota);
                    _gabiotas.eraseObject(gabiota); // Elimina el gabiota de la lista _gabiotas
                    EXP += 5;
                    _expLabel->setString("EXP: " + std::to_string(EXP));
                }

                // Marcar la bala para eliminación
                bulletsToRemove.pushBack(bullet);
                break; // Salimos del bucle de gabiotas porque la bala ya no existe
            }
        }
    }

    // Eliminar las balas marcadas
    for (auto bullet : bulletsToRemove) {
        this->removeChild(bullet);
        _bullets.eraseObject(bullet);
    }

    // Check if all regular gabiotas are dead and no boss gabiota exists
    if (_gabiotas.empty() && gabiotaFlag && !gabiotaBoss) {
        Gabiota* bossGabiota = Gabiota::create();
        if (bossGabiota) {
            bossGabiota->BossMode(); // Activar el modo de jefe
            this->addChild(bossGabiota, 1);
            _gabiotas.pushBack(bossGabiota);
            gabiotaBoss = true;
        }
    }
}

void GameLoopScene::addCuervos(int number) {

    addGabiotas(4);
    addPatos(3);
    for (int i = 0; i < number; ++i) {
        // Generar un tiempo aleatorio entre 0 y 10 segundos
        float randomTime = CCRANDOM_0_1() * 20.0f;

        // Usar un scheduler para llamar una función con un retraso aleatorio
        this->scheduleOnce([this, i](float) {
            Cuervo* cuervo = Cuervo::create();
            if (cuervo) {
                cuervoFlag = true;
                this->addChild(cuervo, 1);
                _cuervos.pushBack(cuervo); // Agregar al vector
                cuervo->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
            }
            }, randomTime, "addCuervo_" + std::to_string(i));
    }
}

void GameLoopScene::cuervoColision() {
    static float tiempoDesdeUltimoDaño = 0.0f; // Tiempo acumulado desde el último daño
    const float intervaloDaño = 0.5f;         // Tiempo mínimo entre daños en segundos

    // Incrementa el tiempo desde el último daño basado en el tiempo transcurrido en el frame actual
    tiempoDesdeUltimoDaño += Director::getInstance()->getDeltaTime();

    if (_bolillo) {
        for (auto cuervo : _cuervos) {
            // Pasa la posición de Bolillo a cada Cuervo
            cuervo->setTargetPosition(_bolillo->getPosition());

            // Detectar colisión
            if (cuervo->getBoundingBox().intersectsRect(_bolillo->getBoundingBox())) {
                // Verificar si el intervalo de tiempo ha pasado
                if (tiempoDesdeUltimoDaño >= intervaloDaño) {
                    // Reducir la vida de _bolillo
                    _bolillo->reduceLife(5);
                    tiempoDesdeUltimoDaño = 0.0f; // Reinicia el tiempo del temporizador

                    // Verificar si Bolillo debe desaparecer
                    if (_bolillo->getLife() <= 0) {
                        goToGameOverScene(false);
                        this->removeChild(_bolillo);
                        _bolillo = nullptr;
                        return; // Salir para evitar accesos inválidos
                    }
                }

                // Si ya se detectó colisión y aplicó daño, no es necesario seguir revisando más
                break;
            }
        }
    }
}

void GameLoopScene::checkCuervoBulletCollisions() {
    // Crear una lista temporal para almacenar balas que deben eliminarse
    Vector<Bullet*> bulletsToRemove;

    for (auto bullet : _bullets) {
        for (auto cuervo : _cuervos) {
            if (bullet->getBoundingBox().intersectsRect(cuervo->getBoundingBox())) {
                // Reducir vida del cuervo
                cuervo->reduceLife(1);

                // Eliminar el cuervo si su vida llega a 0
                if (cuervo->getLife() <= 0) {
                    // Eliminar el gabiota de la escena y de la lista
                    if (cuervo->getScale() > 1.0f && gabiotaBoss) { // Asumimos que el jefe tiene un tamaño mayor
                        // Activar el spawn de cuervos al morir el bossGabiota
                        EXP += 70;
                        goToStoreScene();
                        addPalomas(6); // Ajusta el número de gaviotas que deseas agregar
                    }
                    cocos2d::AudioEngine::play2d("cua.mp3", false);
                    this->removeChild(cuervo);
                    _cuervos.eraseObject(cuervo);
                    EXP += 6;
                    _expLabel->setString("EXP: " + std::to_string(EXP));
                }
                // Marcar la bala para eliminación
                bulletsToRemove.pushBack(bullet);
                break; // Salimos del bucle de cuervos porque la bala ya no existe
            }
        }
    }

    // Eliminar las balas marcadas
    for (auto bullet : bulletsToRemove) {
        this->removeChild(bullet);
        _bullets.eraseObject(bullet);
    }

    // Check if all regular gabiotas are dead and no boss gabiota exists
    if (_cuervos.empty() && cuervoFlag && !cuervoBoss) {
        Cuervo* bossCuervo = Cuervo::create();
        if (bossCuervo) {
            bossCuervo->BossMode(); // Activar el modo de jefe
            this->addChild(bossCuervo, 1);
            _cuervos.pushBack(bossCuervo);
            cuervoBoss = true;
        }
    }
}

void GameLoopScene::addPalomas(int number) {

    addGabiotas(3);
    addCuervos(3);

    for (int i = 0; i < number; ++i) {
        // Generar un tiempo aleatorio entre 0 y 10 segundos
        float randomTime = CCRANDOM_0_1() * 20.0f;

        // Usar un scheduler para llamar una función con un retraso aleatorio
        this->scheduleOnce([this, i](float) {
            Paloma* paloma = Paloma::create();
            if (paloma) {
                palomaFlag = true;
                this->addChild(paloma, 1);
                _palomas.pushBack(paloma); // Agregar al vector
                paloma->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
            }
            }, randomTime, "addPaloma_" + std::to_string(i));
    }
}

void GameLoopScene::palomaColision() {
    static float tiempoDesdeUltimoDaño = 0.0f; // Tiempo acumulado desde el último daño
    const float intervaloDaño = 0.5f;         // Tiempo mínimo entre daños en segundos

    // Incrementa el tiempo desde el último daño basado en el tiempo transcurrido en el frame actual
    tiempoDesdeUltimoDaño += Director::getInstance()->getDeltaTime();

    if (_bolillo) {
        for (auto paloma : _palomas) {
            // Pasa la posición de Bolillo a cada Paloma
            paloma->setTargetPosition(_bolillo->getPosition());

            // Detectar colisión
            if (paloma->getBoundingBox().intersectsRect(_bolillo->getBoundingBox())) {
                // Verificar si el intervalo de tiempo ha pasado
                if (tiempoDesdeUltimoDaño >= intervaloDaño) {
                    // Reducir la vida de _bolillo
                    _bolillo->reduceLife(5);
                    tiempoDesdeUltimoDaño = 0.0f; // Reinicia el tiempo del temporizador

                    // Verificar si Bolillo debe desaparecer
                    if (_bolillo->getLife() <= 0) {
                        this->removeChild(_bolillo);
                        _bolillo = nullptr;
                        goToGameOverScene(false);
                        return; // Salir para evitar accesos inválidos
                    }
                }

                // Si ya se detectó colisión y aplicó daño, no es necesario seguir revisando más
                break;
            }
        }
    }
}

void GameLoopScene::checkPalomaBulletCollisions() {
    // Crear una lista temporal para almacenar balas que deben eliminarse
    Vector<Bullet*> bulletsToRemove;

    for (auto bullet : _bullets) {
        for (auto paloma : _palomas) {
            if (bullet->getBoundingBox().intersectsRect(paloma->getBoundingBox())) {
                // Reducir vida del paloma
                paloma->reduceLife(1);

                // Eliminar el paloma si su vida llega a 0
                if (paloma->getLife() <= 0) {
                    if (paloma->getScale() > 1.0f && palomaBoss) { // Asumimos que el jefe tiene un tamaño mayor
                        // Activar el spawn de cuervos al morir el bossGabiotai
                        EXP += 50;
                        goToStoreScene();
                        addBuitres(7); // Ajusta el número de gaviotas que deseas agregar
                    }
                    cocos2d::AudioEngine::play2d("cua.mp3", false);
                    this->removeChild(paloma);
                    _palomas.eraseObject(paloma);
                    EXP += 8;
                    _expLabel->setString("EXP: " + std::to_string(EXP));
                }
                // Marcar la bala para eliminación
                bulletsToRemove.pushBack(bullet);
                break; // Salimos del bucle de palomas porque la bala ya no existe
            }
        }
    }

    // Eliminar las balas marcadas
    for (auto bullet : bulletsToRemove) {
        this->removeChild(bullet);
        _bullets.eraseObject(bullet);
    }

    // Check if all regular gabiotas are dead and no boss gabiota exists
    if (_palomas.empty() && palomaFlag && !palomaBoss) {
        Paloma* bossPaloma = Paloma::create();
        if (bossPaloma) {
            bossPaloma->BossMode(); // Activar el modo de jefe
            this->addChild(bossPaloma, 1);
            _palomas.pushBack(bossPaloma);
            palomaBoss = true;
        }
    }
}

void GameLoopScene::addBuitres(int number) {

    addGabiotas(2);
    addCuervos(2);
    addPalomas(2);
    for (int i = 0; i < number; ++i) {
        // Generar un tiempo aleatorio entre 0 y 10 segundos
        float randomTime = CCRANDOM_0_1() * 5.0f;

        // Usar un scheduler para llamar una función con un retraso aleatorio
        this->scheduleOnce([this, i](float) {
            Buitre* buitre = Buitre::create();
            if (buitre) {
                buitreFlag = true;
                this->addChild(buitre, 1);
                _buitres.pushBack(buitre); // Agregar al vector
                buitre->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
            }
            }, randomTime, "addBuitre_" + std::to_string(i));
    }
}

void GameLoopScene::buitreColision() {
    static float tiempoDesdeUltimoDaño = 0.0f; // Tiempo acumulado desde el último daño
    const float intervaloDaño = 0.5f;         // Tiempo mínimo entre daños en segundos

    // Incrementa el tiempo desde el último daño basado en el tiempo transcurrido en el frame actual
    tiempoDesdeUltimoDaño += Director::getInstance()->getDeltaTime();

    if (_bolillo) {
        for (auto buitre : _buitres) {
            // Pasa la posición de Bolillo a cada Buitre
            buitre->setTargetPosition(_bolillo->getPosition());

            // Detectar colisión
            if (buitre->getBoundingBox().intersectsRect(_bolillo->getBoundingBox())) {
                // Verificar si el intervalo de tiempo ha pasado
                if (tiempoDesdeUltimoDaño >= intervaloDaño) {
                    // Reducir la vida de _bolillo
                    _bolillo->reduceLife(10);
                    tiempoDesdeUltimoDaño = 0.0f; // Reinicia el tiempo del temporizador

                    // Verificar si Bolillo debe desaparecer
                    if (_bolillo->getLife() <= 0) {
                        this->removeChild(_bolillo);
                        _bolillo = nullptr;
                        goToGameOverScene(false);
                        return; // Salir para evitar accesos inválidos
                    }
                }

                // Si ya se detectó colisión y aplicó daño, no es necesario seguir revisando más
                break;
            }
        }
    }
}

void GameLoopScene::checkBuitreBulletCollisions() {
    // Crear una lista temporal para almacenar balas que deben eliminarse
    Vector<Bullet*> bulletsToRemove;

    for (auto bullet : _bullets) {
        for (auto buitre : _buitres) {
            if (bullet->getBoundingBox().intersectsRect(buitre->getBoundingBox())) {
                // Reducir vida del buitre
                buitre->reduceLife(1);

                if (buitre->getLife() <= 0) {
                    if (buitre->getScale() > 1.0f && buitreBoss) { // Asumimos que el jefe tiene un tamaño mayor
                        // Activar el spawn de cuervos al morir el bossGabiota
                        EXP += 100;
                        goToGameOverScene(true);
                    }
                    cocos2d::AudioEngine::play2d("cua.mp3", false);
                    this->removeChild(buitre);
                    _buitres.eraseObject(buitre);
                    EXP += 10;
                    _expLabel->setString("EXP: " + std::to_string(EXP));
                }
                // Marcar la bala para eliminación
                bulletsToRemove.pushBack(bullet);
                break; // Salimos del bucle de buitres porque la bala ya no existe
            }
        }
    }

    // Eliminar las balas marcadas
    for (auto bullet : bulletsToRemove) {
        this->removeChild(bullet);
        _bullets.eraseObject(bullet);
    }

    // Check if all regular gabiotas are dead and no boss gabiota exists
    if (_buitres.empty() && buitreFlag && !buitreBoss) {
        Buitre* bossBuitre = Buitre::create();
        if (bossBuitre) {
            bossBuitre->BossMode(); // Activar el modo de jefe
            this->addChild(bossBuitre, 1);
            _buitres.pushBack(bossBuitre);
            buitreBoss = true;
        }
    }
}

void GameLoopScene::goToGameOverScene(bool des) {
    auto gameOverScene = GameOverScene::create(EXP, des);
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameOverScene, Color3B::BLACK));
}

void GameLoopScene::goToStoreScene() {
    auto storeScene = StoreScene::create(EXP, _bolillo->getLife(), _bolillo->getSpeed(), this);
    Director::getInstance()->pushScene(TransitionFade::create(1.0f, storeScene, Color3B::BLACK));
}

void GameLoopScene::updatePlayerStats(int life, float speed, long _EXP) {
    _bolillo->setLife(life);
    _bolillo->setSpeed(speed);
    this->setEXP(_EXP);
}

void GameLoopScene::setEXP(long _EXP) {
    EXP = _EXP;
}

CREATE_FUNC(GameLoopScene);