#pragma once
#include "Button.h"

class Gate : public IMobile { // TODO: IActivable
public:
	Gate();
	Gate( Box box, double timerAdd_s, int index );
	virtual ~Gate();
	virtual void collide( ColliderType ownHitbox, ColliderType otherHitbox, IUnique &other ) noexcept override;

    virtual void updateLogic( double dt_s ) noexcept override {} // stub
    virtual void updatePhysics( double dt_s ) noexcept override {} // stub
    virtual void updateHitboxes() noexcept override {} // stub
    virtual void updateGraphics() noexcept override {} // stub
    virtual void updateAnimations( double dt_s, double t_s ) noexcept override {} // stub
    [[nodiscard]] virtual std::variant<Boxes,Spheres> getRepresentation() const noexcept override;

 // TODO:
//    void bindButton( Button *button ) noexcept;
private:
	int     index;
	Button *button;
	double  timer_s, 
            timerAdd_s;
	bool    isMoved;
};
