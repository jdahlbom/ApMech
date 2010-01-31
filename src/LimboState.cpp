#include "LimboState.h"

#include "ActionKeyMap.h"
#include "GameStateManager.h"
#include "Gui.h"
#include "net/netdata.h"
#include "MechData.h"

namespace ap {
    LimboState::LimboState(ap::GameStateManager *gameStateManager,
            ap::Gui *gui, ap::net::NetData *netdata,
            ap::ActionKeyMap *akMap):
        pGui(gui),
        pNetData(netdata),
        pActionKMap(akMap),
        selectedVehicleIndex(0),
        vehicleCount(0)
    {
        assert(NULL != pGui);
        initStateManager(gameStateManager);
        pGui->setupLimboWindow();
    }

    void LimboState::enter( void ) {
        assert(NULL != pGui);
        pGui->setVisibleLimboMenu(true);
	pGui->clearLimboVehicleList();
	selectedVehicleIndex = 0;
	vehicleCount = 0;

        while (ap::MechData *mechData =
                pNetData->eachObject<ap::MechData *>(ap::OBJECT_TYPE_MECHDATA)) {
            pGui->addLimboVehicle(mechData->getName());
            if (vehicleCount == 0) {
                // select the first item
                pGui->selectLimboVehicle(0);
            }
            vehicleCount++;
        }
    }

    void LimboState::exit( void ) {
        assert(NULL != pGui);
        pGui->setVisibleLimboMenu(false);
    }

    void LimboState::pause( void ) {}
    void LimboState::resume( void ) {}
    void LimboState::update( unsigned long lTimeElapsed ) {}

    bool LimboState::keyPressed( const ooinput::KeyEvent &e )
    {
        assert(pNetData != NULL);
        if(pGui->keyPressed(e))
            return true;

        switch (e.key) {
            case ap::ooinput::AP_K_ESCAPE:
                requestShutdown();
                return true;
            case ap::ooinput::AP_K_DOWN:
                if (vehicleCount > 0 && selectedVehicleIndex < vehicleCount-1) {
                    selectedVehicleIndex++;
                    pGui->selectLimboVehicle(selectedVehicleIndex);
                }
                // std::cout << "Received key press (down), index: " << selectedVehicleIndex << std::endl;
                break;
            case ap::ooinput::AP_K_UP:
                if (vehicleCount > 0 && selectedVehicleIndex > 0) {
                    selectedVehicleIndex--;
                    pGui->selectLimboVehicle(selectedVehicleIndex);
                }
                // std::cout << "Received key press (up), index: " << selectedVehicleIndex << std::endl;
                break;
            default:
                break;
        }

        IngameAction action = pActionKMap->getActionForKey(e.key);

        switch( action ) {
            case(TOGGLE_LIMBOMENU):
                if ("" != pNetData->me.chosenVehicleType) {
                    getStateManager()->leaveLimboMenu();
                }
                break;
            case(FIRE_WEAPON):
                {
                    std::string name = pGui->getLimboVehicle(selectedVehicleIndex);
                    std::cout << "Selected vehicle " << name << std::endl;
                    selectVehicleType(name);
                }
                break;
            default:
                // Intentionally empty.
                break;
        }
        return true;
    }
    bool LimboState::keyReleased( const ooinput::KeyEvent &e ) {
        if(pGui->keyReleased(e))
            return true;

        return true;
    }

    bool LimboState::mousePressed(const ooinput::MouseClickedEvent &e) {
        if (pGui->mousePressed(e))
            return true;
        return true;
    }
    bool LimboState::mouseReleased(const ooinput::MouseClickedEvent &e) {
        if (pGui->mouseReleased(e))
            return true;
        return true;
    }
    bool LimboState::mouseMoved(const ooinput::MouseMovedEvent &e) {
        if (pGui->mouseMoved(e))
            return true;
        return true;
    }

    void LimboState::selectVehicleType(const std::string &vehicleType) {
        pNetData->me.chosenVehicleType = vehicleType;
        pNetData->me.setChanged();
        pNetData->sendChanges();

        getStateManager()->leaveLimboMenu();
    }

} // namespace ap
