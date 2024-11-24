
#include "stdafx.h"

#include "Helper.h"
#include "Mis_scripts.h"
#include "General.h"
#include "Displayinfo.h"

//---------------------------------------------------------------------------------------------
void convertOPN(std::stringstream& bufferScripts_OPN1, uint32_t logicOperator, uint32_t  scriptNumber, std::stringstream& bufferScriptsOPN, std::stringstream& bufferScripts)
{
	//Функция выполняющая обратную польскую нотацию
	if (scriptNumber == 1)
	{
		bufferScripts << bufferScripts_OPN1.str();
		bufferScripts_OPN1.str("");
	}
	else
	{ 
		if (logicOperator == 0)
		{
			bufferScriptsOPN << bufferScripts_OPN1.str();
		}
		else
		if (logicOperator == 1)
		{
			bufferScripts << "$&" << '\n' << bufferScriptsOPN.str();
			bufferScriptsOPN.str("");
		}
		else
		if (logicOperator == 2)
		{
			bufferScripts << "$|" << '\n' << bufferScriptsOPN.str();
			bufferScriptsOPN.str("");
		}
	}
	bufferScripts_OPN1.str("");
	return;
}
//---------------------------------------------------------------------------------------------
void reverse_num(uint16_t num1, std::string& str)
{
	str = std::to_string(num1);
	reverse(str.begin(), str.end());
}
//---------------------------------------------------------------------------------------------
void getScriptSize(std::stringstream& outputfilebuffer, std::stringstream& bufferScripts, uint32_t accumulatorNumberScripts)
{
	//для получения размера данных есть метод .size() надо изучить
	// переписать функцию на size_t getScriptSize(const char* begin)
	bufferScripts.seekg(0, std::ios::end);
	size_t size = bufferScripts.tellg();
	outputfilebuffer << "script \"" << accumulatorNumberScripts << "\" size " << size - 2 << '\n';
	outputfilebuffer << bufferScripts.str();
	/*cout << "Размер скрипта: " << size << '\n' << endl;*/
	return;
}
//---------------------------------------------------------------------------------------------

void covertMisScripts(const std::vector<uint8_t>& mis_scripts)
{
	//------------------------------------------------------------------------------
	std::ofstream outputFile("map.000/mis.000/scripts2", std::ios::binary);
	if (!outputFile)
	{
		errorBuildFile();
		return;
	}
	//------------------------------------------------------------------------------
	uint32_t numberofscripts = *(uint32_t*)mis_scripts.data();
	//------------------------------------------------------------------------------
	//Скрипты1
	uint8_t bufferAND[4] = { 1, 0, 0, 0 };
	uint8_t bufferOR[4] = { 2, 0, 0, 0 };
	uint8_t bufferUG[4] = { 3, 0, 0, 0 };
	uint8_t bufferUP[4] = { 4, 0, 0, 0 };
	uint8_t bufferUGL[4] = { 5, 0, 0, 0 };
	uint8_t bufferUPL[4] = { 6, 0, 0, 0 };
	uint8_t bufferPKP[4] = { 9, 0, 0, 0 };
	uint8_t bufferPKF[4] = { 10, 0, 0, 0 };
	uint8_t bufferTE[4] = { 27, 0, 0, 0 };
	uint8_t bufferCD[4] = { 28, 0, 0, 0 };
	uint8_t bufferTMS[4] = { 29, 0, 0, 0 };
	uint8_t bufferUGLper[4] = { 30, 0, 0, 0 };
	uint8_t bufferUPLper[4] = { 31, 0, 0, 0 };
	uint8_t bufferUGper[4] = { 32, 0, 0, 0 };
	uint8_t bufferUPper[4] = { 33, 0, 0, 0 };
	uint8_t bufferAIGB[4] = { 34, 0, 0, 0 };
	uint8_t bufferAIZ1[4] = { 35, 0, 0, 0 };
	uint8_t bufferAIZ2[4] = { 36, 0, 0, 0 };
	uint8_t bufferAIG1[4] = { 37, 0, 0, 0 };
	uint8_t bufferAIG2[4] = { 38, 0, 0, 0 };
	uint8_t bufferOID[4] = { 41, 0, 0, 0 };
	uint8_t bufferGWATA[4] = { 49, 0, 0, 0 };
	uint8_t bufferVIC[4] = { 50, 0, 0, 0 };
	uint8_t bufferMIST[4] = { 51, 0, 0, 0 };

	//Скрипты2
	uint8_t bufferSPPL[4] = { 8, 0, 0, 0 };
	uint8_t bufferETC[4] = { 11, 0, 0, 0 };
	uint8_t bufferSTRT[4] = { 12, 0, 0, 0 };
	uint8_t bufferSTPT[4] = { 13, 0, 0, 0 };
	uint8_t bufferMSTL[4] = { 14, 0, 0, 0 };
	uint8_t bufferSP[4] = { 15, 0, 0, 0 };
	uint8_t bufferSCD[4] = { 16, 0, 0, 0 };
	uint8_t bufferSNM[4] = { 17, 0, 0, 0 };
	uint8_t bufferTM[4] = { 18, 0, 0, 0 };
	uint8_t bufferSGB[4] = { 19, 0, 0, 0 };
	uint8_t bufferSGL1[4] = { 20, 0, 0, 0 };
	uint8_t bufferSGL2[4] = { 21, 0, 0, 0 };
	uint8_t bufferSGG1[4] = { 22, 0, 0, 0 };
	uint8_t bufferSGG2[4] = { 23, 0, 0, 0 };
	uint8_t bufferAPP[4] = { 24, 0, 0, 0 };
	uint8_t bufferAFP[4] = { 25, 0, 0, 0 };
	uint8_t bufferRU[4] = { 39, 0, 0, 0 };
	uint8_t bufferSRFS[4] = { 40, 0, 0, 0 };
	uint8_t bufferSPTO[4] = { 42, 0, 0, 0 };
	uint8_t bufferSAT[4] = { 43, 0, 0, 0 };
	uint8_t bufferARPO[4] = { 44, 0, 0, 0 };
	uint8_t bufferARPO2[4] = { 45, 0, 0, 0 };
	uint8_t bufferSPPA[4] = { 46, 0, 0, 0 };
	uint8_t bufferLCCV[4] = { 47, 0, 0, 0 };
	uint8_t bufferMO[4] = { 48, 0, 0, 0 };
	uint8_t bufferSRES[4] = { 52, 0, 0, 0 };
	uint8_t bufferFRES[4] = { 53, 0, 0, 0 };
	uint8_t bufferPFF[4] = { 54, 0, 0, 0 };
	uint8_t bufferFPFF[4] = { 55, 0, 0, 0 };
	uint8_t bufferMFF[4] = { 56, 0, 0, 0 };
	uint8_t bufferSPPO[4] = { 57, 0, 0, 0 };
	uint8_t bufferDGP[4] = { 58, 0, 0, 0 };

	//Старт, конец
	uint8_t bufferEND[4] = { 7, 0, 0, 0 };
	//uint8_t bufferstart[4] = { 67, 65, 79, 70 };
	uint8_t bufferEND2[4] = { 255, 255, 255, 127 };
	//------------------------------------------------------------------------------
	std::stringstream bufferScripts;
	std::stringstream bufferScriptsOPN;
	std::stringstream bufferScripts_OPN1;
	std::stringstream outputFileBuffer;
	//------------------------------------------------------------------------------
	uint32_t accumulatorNumberScripts = 1;
	//------------------------------------------------------------------------------
	
	size_t curOffset{ 0 };
	while(numberofscripts--)
	{
		const uint8_t* buffer = mis_scripts.data() + curOffset;
		curOffset += 8;
		uint8_t sizescripts1 = *(uint8_t*)(buffer + 0);
		uint8_t sizescripts2 = *(uint8_t*)(buffer + 1);
		uint8_t sizescripts3 = *(uint8_t*)(buffer + 2);
		uint8_t sizescripts4 = *(uint8_t*)(buffer + 3);

		uint8_t startscripts1 = *(uint8_t*)(buffer + 4);
		uint8_t startscripts2 = *(uint8_t*)(buffer + 5);
		uint8_t startscripts3 = *(uint8_t*)(buffer + 6);
		uint8_t startscripts4 = *(uint8_t*)(buffer + 7);
		uint32_t sizescripts = ((sizescripts4 & 0xFF) << 24) | ((sizescripts3 & 0xFF) << 16) | ((sizescripts2 & 0xFF) << 8) | (sizescripts1 & 0xFF);

		uint32_t numberoflines = (sizescripts - 4) / 4; //расчет количества циклов в одном скрипте
		std::map <uint32_t, std::string> numberscripts;
		uint32_t n = 0; //присвоение ключа к map
		uint32_t key = 0; //ключ к map
		uint32_t scriptNumber = 0; //номер собранных скриптовых комманд в одном скрипте, нужно для выполнения обратной польской нотации
		
			while (numberoflines--)
			{
				uint8_t logicOperator = 0; //Логический оператор И ИЛИ

				const uint8_t* buffer = mis_scripts.data() + curOffset;
				curOffset += 4;

				uint8_t num1 = *(uint8_t*)(buffer + 0);
				uint8_t num2 = *(uint8_t*)(buffer + 1);
				uint8_t num3 = *(uint8_t*)(buffer + 2);
				uint8_t num4 = *(uint8_t*)(buffer + 3);
				{
					//Переменные значения
					if (num4 == 128)
					{ 
						if (num2 == 0)
						{
							// Реверс числа
							std::string str;
							reverse_num(num1, str);
							//outputFile << "   " << num1 << "   " << num2 << "   " << num3 << "   " << num4 << '\n';
							numberscripts[n] = str;
							//std::cout << key << ": " << str << std::endl;
							n++;
						}
						else
						{
							// Реверс числа
							std::string str;
							uint16_t num12 = ((num2 & 0xFF) << 8) | (num1 & 0xFF);
							//char buffertwobytes[2] = { num1, num2 };
							//SHORT num12 = *(unsigned short*)(buffertwobytes + 0);
							reverse_num(num12, str);
							//outputFile << "   " << num1 << "   " << num2 << "   " << num3 << "   " << num4 << '\n';
							numberscripts[n] = str;
							//std::cout << key << ": " << str << std::endl;
							n++;
						}
		
					}
					else // Операция с координатой ячеек памяти
					if (num2 == 255 && num3 == 255 && num4 == 255)
					{
						// Реверс числа
						std::string str;
						uint32_t num1234 = 392 + num1;
						reverse_num(num1234, str);
						//outputFile << "   " << num1 << "   " << num2 << "   " << num3 << "   " << num4 << '\n';
						numberscripts[n] = str;
						//std::cout << key << ": " << str << std::endl;
						n++;
					}

					//Скрипты в первом окне редактора карт QED (они же подвержены обратной польской нотации)
					else // 1, 0, 0, 0
						if (memcmp(buffer, bufferAND, 4) == 0)
						{
							//Если оператор "И"
							logicOperator = 1;
						}
					else // 2, 0, 0, 0
						if(memcmp(buffer, bufferOR, 4) == 0)
						{ 
							//Если оператор "ИЛИ"
							logicOperator = 2;
						}
					else // 3, 0, 0, 0
						if (memcmp(buffer, bufferUG, 4) == 0)
						{
							bufferScripts_OPN1 << "$ug" << '\n' << "$UMSK" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$ug1" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$sp_8" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$sp_9" << '\n' << "$N1" << '\n' << "#" << numberscripts[key + 3] << '\n';
							key += 4;
							scriptNumber += 1;
						}
					else // 4, 0, 0, 0
						if (memcmp(buffer, bufferUP, 4) == 0)
						{
							bufferScripts_OPN1 << "$up" << '\n' << "$UMSK" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$up1" << '\n' << "$Plr" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$sp_b" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$sp_c" << '\n' << "$N1" << '\n' << "#" << numberscripts[key + 3] << '\n';
							key += 4;
							scriptNumber += 1;
						}
					else // 5, 0, 0, 0
						if (memcmp(buffer, bufferUGL, 4) == 0)
						{
							bufferScripts_OPN1 << "$ugl" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$N1" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$umx0" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$ugl1" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 3] << '\n';
							key += 4;
							scriptNumber += 1;
						}
					else // 6, 0, 0, 0
						if (memcmp(buffer, bufferUPL, 4) == 0)
						{
							bufferScripts_OPN1 << "$upl" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$N1" << '\n' << "#" << numberscripts[key + 1] 
								<< '\n' << "$umx2" << '\n' << "$Plr" << '\n' << "#" << numberscripts[key + 2] 
								<< '\n' << "$upl1" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 3] << '\n';
							key += 4;
							scriptNumber += 1;
						}
					else // 9, 0, 0, 0
						if (memcmp(buffer, bufferPKP, 4) == 0)
						{
							bufferScripts_OPN1 << "$pkp" << '\n' << "$Plr" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$pkp1" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$sp_d" << '\n' << "$N1" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$sp_e" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 3] << '\n';
							key += 4;
							scriptNumber += 1;
						}
					else // 10, 0, 0, 0
						if (memcmp(buffer, bufferPKF, 4) == 0)
						{
							bufferScripts_OPN1 << "$pkf" << '\n' << "$Plr" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$pkf1" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$sp_f" << '\n' << "$N1" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$pkf2" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 3] << '\n';
							key += 4;
							scriptNumber += 1;
						}
					else // 27, 0, 0, 0
						if (memcmp(buffer, bufferTE, 4) == 0)
						{
							bufferScripts_OPN1 << "$te" << '\n' << "$Tr" << '\n' << "#" << numberscripts[key] << '\n';
							key++;
							scriptNumber += 1;
						}
					else // 28, 0, 0, 0
						if (memcmp(buffer, bufferCD, 4) == 0)
						{
							bufferScripts_OPN1 << "$cd" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$sp_5" << '\n' << "$T" << '\n' << "#" << numberscripts[key + 1] << '\n';
							key += 2;
							scriptNumber += 1;
						}
					else // 29, 0, 0, 0
						if (memcmp(buffer, bufferTMS, 4) == 0)
						{
							bufferScripts_OPN1 << "$tms" << '\n' << "$CAB" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$sp_6" << '\n' << "$T" << '\n' << "#" << numberscripts[key + 1] << '\n';
							key += 2;
							scriptNumber += 1;
						}
					else // 30, 0, 0, 0
						if (memcmp(buffer, bufferUGLper, 4) == 0)
						{
							bufferScripts_OPN1 << "$ugl%" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$N1" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$umx1" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$ugl1" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 3] << '\n';
							key += 4;
							scriptNumber += 1;
						}
					else // 31, 0, 0, 0
						if (memcmp(buffer, bufferUPLper, 4) == 0)
						{
							bufferScripts_OPN1 << "$upl%" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$N1" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$umx3" << '\n' << "$Plr" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$upl1" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 3] << '\n';
							key += 4;
							scriptNumber += 1;
						}
					else // 32, 0, 0, 0
						if (memcmp(buffer, bufferUGper, 4) == 0)
						{
							bufferScripts_OPN1 << "$ug%u" << '\n' << "$UMSK" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$umx4" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$sp_7" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$sp_k" << '\n' << "$N1" << '\n' << "#" << numberscripts[key + 3]
								<< '\n' << "$ugut1" << '\n' << "$UMSK" << '\n' << "#" << numberscripts[key + 4]
								<< '\n' << "$ug%u1" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key + 5] << '\n';
							key += 6;
							scriptNumber += 1;
							}
					else // 33, 0, 0, 0
						if (memcmp(buffer, bufferUPper, 4) == 0)
						{
							bufferScripts_OPN1 << "$up%" << '\n' << "$UMSK" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$up%u1" << '\n' << "$Plr" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$sp_a" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$sp_k" << '\n' << "$N1" << '\n' << "#" << numberscripts[key + 3]
								<< '\n' << "$ugut1" << '\n' << "$UMSK" << '\n' << "#" << numberscripts[key + 4]
								<< '\n' << "$up%u2" << '\n' << "$Plr" << '\n' << "#" << numberscripts[key + 5] << '\n';
							key += 6;
							scriptNumber += 1;
						}
					else // 34, 0, 0, 0
						if (memcmp(buffer, bufferAIGB, 4) == 0)
						{
							bufferScripts_OPN1 << "$aigb" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$aigb1" << '\n' << "$AI" << '\n' << "#" << numberscripts[key + 1] << '\n';
							key += 2;
							scriptNumber += 1;
						}
					else // 35, 0, 0, 0
						if (memcmp(buffer, bufferAIZ1, 4) == 0)
						{
							bufferScripts_OPN1 << "$aiz1" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key]
									<< '\n' << "$aiz_1" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 1] << '\n';
							key += 2;
							scriptNumber += 1;
						}
					else // 36, 0, 0, 0
						if (memcmp(buffer, bufferAIZ2, 4) == 0)
						{
							bufferScripts_OPN1 << "$aiz2" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$aiz_2" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 1] << '\n';
							key += 2;
							scriptNumber += 1;
						}
					else // 37, 0, 0, 0
						if (memcmp(buffer, bufferAIG1, 4) == 0)
						{
							bufferScripts_OPN1 << "$aig1" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$aig_1" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key + 1] << '\n';
							key += 2;
							scriptNumber += 1;
						}
					else // 38, 0, 0, 0
						if (memcmp(buffer, bufferAIG2, 4) == 0)
						{
							bufferScripts_OPN1 << "$aig2" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$aig_2" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key + 1] << '\n';
							key += 2;
							scriptNumber += 1;
						}
					else // 41, 0, 0, 0
						if (memcmp(buffer, bufferOID, 4) == 0)
						{
							bufferScripts_OPN1 << "$oid" << '\n' << "$Obj" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$oid1" << '\n';
							key ++;
							scriptNumber += 1;
						}
					else // 49, 0, 0, 0
						if (memcmp(buffer, bufferGWATA, 4) == 0)
						{
							bufferScripts_OPN1 << "$gwata" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$gwata1" << '\n' << "$CAB" << '\n' << "#" << numberscripts[key + 1] 
								<< '\n' << "$T" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$gwata2" << '\n';
							key += 3;
							scriptNumber += 1;
						}
					else // 50, 0, 0, 0
						if (memcmp(buffer, bufferVIC, 4) == 0)
						{
							bufferScripts_OPN1 << "$vic" << '\n' << "$Lvar" << '\n' << "#" << numberscripts[key] << NUMMEMORY
								<< '\n' << "$CABE" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$Rvar" << '\n' << "#" << numberscripts[key + 2] << '\n';
							key += 3;
							scriptNumber += 1;
						}
					else // 51, 0, 0, 0
						if (memcmp(buffer, bufferMIST, 4) == 0)
						{
							bufferScripts_OPN1 << "$mist" << '\n';
							scriptNumber += 1;
						}


					//Скрипты во втором окне редактора карт QED
					else // 8, 0, 0, 0
						if (memcmp(buffer, bufferSPPL, 4) == 0)
						{
							bufferScripts << "$sppl" << '\n' << "$I" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$spc" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 1] 
								<< '\n' << "$sppl1" << '\n' << "$P" << '\n' << "#" << numberscripts[key + 2] 
								<< '\n' << "$sppl2" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 3] 
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 4;
						}
					else // 11, 0, 0, 0 
						if (memcmp(buffer, bufferETC, 4) == 0)
						{
							bufferScripts << "$etc" << '\n' << "$" << "\\\\" << "n" << '\n';
						}
					else // 12, 0, 0, 0
						if (memcmp(buffer, bufferSTRT, 4) == 0)
						{
							bufferScripts << "$strt" << '\n' << "$Tr" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$strt1" << '\n' << "$T" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 2;
						}
					else // 13, 0, 0, 0 
						if (memcmp(buffer, bufferSTPT, 4) == 0)
						{
							bufferScripts << "$stpt" << '\n' << "$Tr" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key++;
						}
					else // 14, 0, 0, 0
						if (memcmp(buffer, bufferMSTL, 4) == 0)
						{
							bufferScripts << "$mstl" << '\n' << "$L" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key++;
						}
					else // 15, 0, 0, 0
						if (memcmp(buffer, bufferSP, 4) == 0)
						{
							bufferScripts << "$sp" << '\n' << "$phrs" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key++;
						}
					else // 16, 0, 0, 0 
						if (memcmp(buffer, bufferSCD, 4) == 0)
						{
							bufferScripts << "$scd" << '\n' << "$T" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key++;
						}
					else // 17, 0, 0, 0 
						if (memcmp(buffer, bufferSNM, 4) == 0)
						{
							bufferScripts << "$snm" << '\n' << "$mission" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key++;
						}
					else // 18, 0, 0, 0
						if (memcmp(buffer, bufferTM, 4) == 0)
						{
							bufferScripts << "$tm" << '\n' << "$F" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key++;
						}
					else // 19, 0, 0, 0
						if (memcmp(buffer, bufferSGB, 4) == 0)
						{
							bufferScripts << "$sgb" << '\n' << "$G" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$sgb1" << '\n' << "$AI" << '\n' << "#" << numberscripts[key + 1] 
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 2;
						}
					else // 20, 0, 0, 0	
						if (memcmp(buffer, bufferSGL1, 4) == 0)
						{
							bufferScripts << "$sgl1" << '\n' << "$G" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$sgl11" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 1] 
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 2;
						}
					else // 21, 0, 0, 0	
						if (memcmp(buffer, bufferSGL2, 4) == 0)
						{
							bufferScripts << "$sgl2" << '\n' << "$G" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$sgl21" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 1] 
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 2;
						}
					else // 22, 0, 0, 0			
						if (memcmp(buffer, bufferSGG1, 4) == 0)
						{
							bufferScripts << "$sgg1" << '\n' << "$G" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$sgg11" << '\n' << "$G" << '\n' << "#" << numberscripts[key + 1] 
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 2;
						}
					else // 23, 0, 0, 0
						if (memcmp(buffer, bufferSGG2, 4) == 0)
						{
							bufferScripts << "$sgg2" << '\n' << "$G" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$sgg21" << '\n' << "$G" << '\n' << "#" << numberscripts[key + 1] 
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 2;
						}
					else // 24, 0, 0, 0
						if (memcmp(buffer, bufferAPP, 4) == 0)
						{
							bufferScripts << "$app" << '\n' << "$P" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$spc" << '\n' << "$I" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$spc" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 3;
						}
					else // 25, 0, 0, 0
						if (memcmp(buffer, bufferAFP, 4) == 0)
						{
							bufferScripts << "$afp" << '\n' << "$P" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$spc" << '\n' << "$I" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$afp1" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 3;
						}
					else // 39, 0, 0, 0
						if (memcmp(buffer, bufferRU, 4) == 0)
						{
							bufferScripts << "$ru" << '\n' << "$G " << '\n' << "#" << numberscripts[key]
								<< '\n' << "$ru1" << '\n' << "$flg" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$ru2" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 2] 
								<< '\n' << "$ru3" << '\n' << "$T" << '\n' << "#" << numberscripts[key + 3] 
								<< '\n' << "$ru4" << '\n' << "$I" << '\n' << "#" << numberscripts[key + 4] 
								<< '\n' << "$ru5" << '\n' << "$I" << '\n' << "#" << numberscripts[key + 5] 
								<< '\n' << "$ru5" << '\n' << "$I" << '\n' << "#" << numberscripts[key + 6] 
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 7;
						}
					else // 40, 0, 0, 0
						if (memcmp(buffer, bufferSRFS, 4) == 0)
						{
							bufferScripts << "$srfs" << '\n' << "$P" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$srfs0" << '\n' << "$resv" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$srfs1" << '\n' << "$flg" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$srfs2" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 3]
								<< '\n' << "$srfs3" << '\n' << "$T" << '\n' << "#" << numberscripts[key + 4]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 5;
						}
					else // 42, 0, 0, 0
						if (memcmp(buffer, bufferSPTO, 4) == 0)
						{
							bufferScripts << "$spto" << '\n' << "$I" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$spc" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$spto1" << '\n' << "$P" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$spto2" << '\n' << "$Obj" << '\n' << "#" << numberscripts[key + 3]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 4;
						}
					else // 43, 0, 0, 0
						if (memcmp(buffer, bufferSAT, 4) == 0)
						{
							bufferScripts << "$sat" << '\n' << "$" << "\\\\" << "n" << '\n';;
						}
					else // 44, 0, 0, 0
						if (memcmp(buffer, bufferARPO, 4) == 0)
						{
							bufferScripts << "$arpo" << '\n' << "$Obj" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key++;
						}
					else // 45, 0, 0, 0
						if (memcmp(buffer, bufferARPO2, 4) == 0)
						{
							bufferScripts << "$arpo" << '\n' << "$Obj" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key++;
						}
					else // 46, 0, 0, 0
						if (memcmp(buffer, bufferSPPA, 4) == 0)
						{
							bufferScripts << "$sppa" << '\n' << "$I" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$spc" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$sppa1" << '\n' << "$P" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$sppa2" 
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 3;
						}
					else // 47, 0, 0, 0 скрипт не до конца рабочий
						if (memcmp(buffer, bufferLCCV, 4) == 0)
						{
							bufferScripts << "$lccv" << '\n' << "$Lvar" << '\n' << "#" << numberscripts[key] << NUMMEMORY
								<< '\n' << "$lccv1" << '\n' << "$Rvar" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 2;
							//$Rvar не расшифрован
						}
					else // 48, 0, 0, 0 скрипт не до конца рабочий
						if (memcmp(buffer, bufferMO, 4) == 0)
						{
							bufferScripts << "$mo" << '\n' << "$Lvar" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$spc" << '\n' << "$Math" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$spc" << '\n' << "$Rvar" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 3;
							//$Math не расшифрован
						}
					else // 52, 0, 0, 0
						if(memcmp(buffer, bufferSRES, 4) == 0)
						{
							bufferScripts << "$sres" << '\n' << "$resv" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$sres1" << '\n' << "$resv" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$sres2" << '\n' << "$resv" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$sres3" << '\n' << "$T" << '\n' << "#" << numberscripts[key + 3]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 4;
						}
					else // 53, 0, 0, 0
						if (memcmp(buffer, bufferFRES, 4) == 0)
						{
							bufferScripts << "$fres" << '\n' << "$Fmask" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$fres0" << '\n' << "$Fmask" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$fres1" << '\n' << "$Fmask" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$fres2" << '\n' << "$T" << '\n' << "#" << numberscripts[key + 3]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 4;
						}
					else // 54, 0, 0, 0
						if (memcmp(buffer, bufferPFF, 4) == 0)
						{
							bufferScripts << "$pff" << '\n' << "$Fmask" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$spc" << '\n' << "$I" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$spc" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 3;
						}
					else // 55, 0, 0, 0
						if (memcmp(buffer, bufferFPFF, 4) == 0)
						{
							bufferScripts << "$fpff" << '\n' << "$Fmask" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$spc" << '\n' << "$I" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$fpff1 " << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 2]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 3;
						}
					else // 56, 0, 0, 0
						if (memcmp(buffer, bufferMFF, 4) == 0)
						{
							bufferScripts << "$mff" << '\n' << "$Fmask" << '\n' << "#" << numberscripts[key]
								<< '\n' << "$spc" << '\n' << "$phrs" << '\n' << "#" << numberscripts[key + 1]
								<< '\n' << "$" << "\\\\" << "n" << '\n';
							key += 2;
						}
					else // 57, 0, 0, 0
						if (memcmp(buffer, bufferSPPO, 4) == 0)
						{
							bufferScripts << "$sppo" << '\n' << "$phrs" << '\n' << "#" << numberscripts[key]
									<< '\n' << "$sppo1" << '\n' << "$Obj" << '\n' << "#" << numberscripts[key + 1]
									<< '\n' << "$" << "\\\\" << "n" << '\n';
								key += 2;
						}
					else // 58, 0, 0, 0
						if (memcmp(buffer, bufferDGP, 4) == 0)
						{
							bufferScripts << "$dgp" << '\n' << "$G" << '\n' << "#" << numberscripts[key]
									<< '\n' << "$dgp1" << '\n' << "$P" << '\n' << "#" << numberscripts[key + 1]
									<< '\n' << "$" << "\\\\" << "n" << '\n';
								key += 2;
						}

					//END - разделитель между двумя окнами скриптов в редакторе карт QED
					else // 7, 0, 0, 0
						if (memcmp(buffer, bufferEND, 4) == 0)
						{
							bufferScripts << "$END" << '\n' << "!" << '\n';
						}
					//END - конец скрипта
					else // 255, 255, 255, 127
						if (memcmp(buffer, bufferEND2, 4) == 0)
						{
							bufferScripts << "$END" << '\n' << '\r' << '\n' ;
						}


					//Неизвестные скрипты
					else	
						{
							bufferScripts << "   " << num1 << "   " << num2 << "   " << num3 << "   " << num4 << '\n';
						}
						convertOPN(bufferScripts_OPN1, logicOperator, scriptNumber, bufferScriptsOPN, bufferScripts);
				}

			}
			
			
		getScriptSize(outputFileBuffer, bufferScripts, accumulatorNumberScripts);
		outputFile << outputFileBuffer.str();
		bufferScripts.str("");
		outputFileBuffer.str("");
		numberscripts.clear();
		accumulatorNumberScripts++;
	}
	outputFile.close();
	return;
}
