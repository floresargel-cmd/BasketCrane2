plcStepTexts.add(craneConveyorClass::plcStepIdle			    ,tr("Idle")			                                    ,tr("Plc status:%1\nIdle").arg(craneConveyorClass::plcStepIdle)	);
plcStepTexts.add(craneConveyorClass::plcStepStart				,tr("Active mission")									,tr("Plc status:%1\nActive mission").arg(craneConveyorClass::plcStepStart)	);
plcStepTexts.add(craneConveyorClass::plcStepGoToXYLoad			,tr("Go to xy target load position")					,tr("Plc status:%1\nGo to xy target load position").arg(craneConveyorClass::plcStepGoToXYLoad)	);
plcStepTexts.add(craneConveyorClass::plcStepGoToZLoad			,tr("Go to z target load position")						,tr("Plc status:%1\nGo to z target load position").arg(craneConveyorClass::plcStepGoToZLoad)	);
plcStepTexts.add(craneConveyorClass::plcStepPinsGetBasket	    ,tr("Pins out to get basket")							,tr("Plc status:%1\nPins out to get basket").arg(craneConveyorClass::plcStepPinsGetBasket)	);
plcStepTexts.add(craneConveyorClass::plcStepGoTo0Z			    ,tr("Go to 0 Z")					                    ,tr("Plc status:%1\nGo to 0 Z").arg(craneConveyorClass::plcStepGoTo0Z)	);
plcStepTexts.add(craneConveyorClass::plcStepGoToXYUnLoad		,tr("Go to xy target unload position with basket")		,tr("Plc status:%1\nGo to xy target unload position with basket").arg(craneConveyorClass::plcStepGoToXYUnLoad)	);
plcStepTexts.add(craneConveyorClass::plcStepGoToZUnload			,tr("Go to z target unload position with basket")		,tr("Plc status:%1\nGo to z target unload position with basket").arg(craneConveyorClass::plcStepGoToZUnload)	);
plcStepTexts.add(craneConveyorClass::plcStepPinsReleaseBasket	,tr("Pins in to release basket")						,tr("Plc status:%1\nPins in to release basket").arg(craneConveyorClass::plcStepPinsReleaseBasket)	);
plcStepTexts.add(craneConveyorClass::plcStepGoToZeroZ			,tr("Go to 0 Z")									    ,tr("Plc status:%1\nGo to 0 Z").arg(craneConveyorClass::plcStepGoToZeroZ)	);
plcStepTexts.add(craneConveyorClass::plcStepFinished		    ,tr("Mission finished")									,tr("Plc status:%1\nThe mission finished succesfully").arg(craneConveyorClass::plcStepFinished)	);


plcStepIdle=0,
plcStepStart=1,
plcStepGoToXYLoad=2,
plcStepGoToZLoad=3,
plcStepPinsGetBasket=4			
plcStepGoTo0Z=5,
plcStepGoToXYUnLoad=6,
plcStepGoToZUnload=7,
plcStepPinsReleaseBasket=8
plcStepGoToZeroZ=9
plcStepFinished=10,
		