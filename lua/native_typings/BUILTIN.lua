---@param TscriptName const char*
---@param Targs Any*
---@param TargCount int
---@param TstackSize int
---@return int
function START_NEW_SCRIPT_WITH_ARGS(TscriptName,Targs,TargCount,TstackSize)  end
---@param Tx float
---@param Ty float
---@param Tz float
---@return float
function VMAG(Tx,Ty,Tz)  end
---@param Tx float
---@param Ty float
---@param Tz float
---@return float
function VMAG2(Tx,Ty,Tz)  end
---@param Tvalue int
---@return void
function SETTIMERB(Tvalue)  end
---@param Tvalue float
---@return float
function SQRT(Tvalue)  end
---@param Tx1 float
---@param Ty1 float
---@param Tz1 float
---@param Tx2 float
---@param Ty2 float
---@param Tz2 float
---@return float
function VDIST(Tx1,Ty1,Tz1,Tx2,Ty2,Tz2)  end
---@param Tpriority int
---@return void
function _SET_THREAD_PRIORITY(Tpriority)  end
---@return int
function TIMERB()  end
---@param TscriptHash Hash
---@param TstackSize int
---@return int
function START_NEW_SCRIPT_WITH_NAME_HASH(TscriptHash,TstackSize)  end
---@param Tvalue int
---@return float
function TO_FLOAT(Tvalue)  end
---@param Tvalue float
---@return int
function ROUND(Tvalue)  end
---@param Tvalue float
---@return int
function CEIL(Tvalue)  end
---@param Tvalue float
---@return int
function FLOOR(Tvalue)  end
---@param Tvalue int
---@param TbitShift int
---@return int
function SHIFT_RIGHT(Tvalue,TbitShift)  end
---@param Tvalue int
---@param TbitShift int
---@return int
function SHIFT_LEFT(Tvalue,TbitShift)  end
---@param TscriptHash Hash
---@param Targs Any*
---@param TargCount int
---@param TstackSize int
---@return int
function START_NEW_SCRIPT_WITH_NAME_HASH_AND_ARGS(TscriptHash,Targs,TargCount,TstackSize)  end
---@return float
function TIMESTEP()  end
---@param Tms int
---@return void
function WAIT(Tms)  end
---@param TscriptName const char*
---@param TstackSize int
---@return int
function START_NEW_SCRIPT(TscriptName,TstackSize)  end
---@return int
function TIMERA()  end
---@param Tx1 float
---@param Ty1 float
---@param Tz1 float
---@param Tx2 float
---@param Ty2 float
---@param Tz2 float
---@return float
function VDIST2(Tx1,Ty1,Tz1,Tx2,Ty2,Tz2)  end
---@param Tvalue int
---@return void
function SETTIMERA(Tvalue)  end
---@param Tbase float
---@param Texponent float
---@return float
function POW(Tbase,Texponent)  end
---@param Tvalue float
---@return float
function SIN(Tvalue)  end
---@param Tvalue float
---@return float
function COS(Tvalue)  end
