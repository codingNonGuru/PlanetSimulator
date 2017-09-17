#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include "Types.hpp"

class GameObject;
class Ship;

class Controller {
protected:
	GameObject* target_;
	Ship* parent_;
	bool actions_[(int)Actions::COUNT];
	bool missions_[(int)Missions::COUNT];
public:
	virtual void update() {}
	bool IsActing(Actions action);
	void SetAction(Actions, bool);
	bool GetMission(Missions);
	void SetMission(Missions, bool);
	void Initialize(Ship*);
	Controller() {}
	virtual ~Controller();
	virtual bool isHuman() const {return false;}
	void ReachTarget(float, float);
	void ComputeDistance();
	bool IsIdle();
};

class HumanController : public Controller {
	void update() override;
	bool isHuman() const override {return true;}
};

class MachineController : public Controller {
	virtual void update() override;
	virtual bool isHuman() const override {return false;}
};

class BargeController : public MachineController
{
	void update() override;
};

#endif
