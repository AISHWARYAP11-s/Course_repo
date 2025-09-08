void CalculateCifSigFromVehSig2ms_Veh(void)
{
    /* Local variable declaration and assignment */

    typF32 rms_current = 0.f;
    typF32 rms_current_id = 0.f;
    typF32 rms_current_iq = 0.f;
	typF32 spd_act_gearbox = 0.f;

    /* Module code */

    /* Calculate phase current root mean square value of current from dq \       */
    /* currents of vehicle signal and map to core component signal               */

    rms_current_id = s_Veh_IdActEm1;
    rms_current_iq = s_Veh_IqActEm1;
    rms_current = sqrtf((rms_current_id * rms_current_id) + (rms_current_iq * rms_current_iq)) * VEH_PHASE_CURRENT_RMS;
    s_Cif_IRmsUphPe1.New = F32LimitF32F32F32_Util(rms_current, 0.f, 1000.f);
    s_Cif_IRmsVphPe1.New = s_Cif_IRmsUphPe1.New;
    s_Cif_IRmsWphPe1.New = s_Cif_IRmsUphPe1.New;

    /* Calculate actual gearbox output shaft speed [ILM IDs: 18933697, 18933700*/	
	switch (s_Cif_HardwareVariant)
	{
		case HARDWARE_VARIANT_FSPL:
			spd_act_gearbox = (s_Veh_SpdActWheelLeftMean.New + s_Veh_SpdActWheelRightMean.New) * VEH_SCALE_GBX_OPS_SPD_FACTOR;
		break;

		case HARDWARE_VARIANT_RTMID_LEFT:
		case HARDWARE_VARIANT_RTHIGH_LEFT:
			spd_act_gearbox = s_Veh_SpdActWheelLeftMean.New;
			break;

		case HARDWARE_VARIANT_RTMID_RIGHT:
		case HARDWARE_VARIANT_RTHIGH_RIGHT:
			spd_act_gearbox = s_Veh_SpdActWheelRightMean.New;
			break;

		default:
			spd_act_gearbox = F32_ZERO;
			break;
	}
    
	if (s_Cif_SpdActGearbox.Status == S_SIGNAL_VALID)
    {
		s_Cif_SpdActGearbox.New = spd_act_gearbox;
	}
	
    else
    {
        s_Cif_SpdActGearbox.New = c_Veh_SpdActGbxOpsReplVal;
    }

    /* Assign cif signals to vehicle variables (will be removed with CA 18607488 Refactoring of ParklockCtrlCommunication)*/
    /* Gbx actual ops speed */
    s_Veh_SpdActGearbox.New = s_Cif_SpdActGearbox.New;
}
