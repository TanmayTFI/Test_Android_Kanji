//====================================
// Framework
// UpdateList Entity Class
//====================================

#ifndef FRMWRK_UPDATELISTENTITY_DEF
#define FRMWRK_UPDATELISTENTITY_DEF

#include "UpdateList.h"
#include <functional>

class UpdateListEntity
{
	public:
		UpdateListEntity()
		{
			UpdateValue = UpdateList::Instance->Add(this);
		}
		virtual ~UpdateListEntity()
		{
			UpdateList::Instance->Remove(UpdateValue);
		}

		virtual void Update(float ds) = 0;

		virtual int GetUpdateValue()
		{
			return UpdateValue;
		}
		

	protected:
		int UpdateValue;
};


class CallFunction : public UpdateListEntity
{
private:

	//UpdateListEntity * _target;
	std::function<void (UpdateListEntity*)> _callback;
	UpdateListEntity * _callbackObject;
	UpdateListEntity * _functionTargetObject;
	float _delayTime;
	bool _isActive;


	public:	
		
		CallFunction(float delayTime,std::function<void (UpdateListEntity*)> callback, UpdateListEntity * functionTargetObject, UpdateListEntity * callbackObject = NULL)
		{
			_callback = callback;
			_callbackObject = callbackObject;
			_functionTargetObject = functionTargetObject;
			_delayTime = delayTime;
			_isActive = false;
		}

		void Update(float ds)
		{
			if(!_isActive)
			{
				return;
			}
			_delayTime -= ds;
			if (_delayTime <= 0)
			{
			//	UpdateListEntity * obj = _callback._Target;
				
				if (_functionTargetObject)
				{
					UpdateListEntity * callbackValue = NULL;
					if (_callbackObject)
					{
						callbackValue = _callbackObject;
					}
					else
					{
						callbackValue = this;
					}
					_callback(callbackValue);
					delete this;
				}
				else
				{
					delete this;
				}
			}
		}

		void Run()
		{
			_isActive = true;
		}
};


#endif