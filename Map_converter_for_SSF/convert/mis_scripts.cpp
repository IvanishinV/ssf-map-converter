#include "mis_scripts.h"
#include "../Helper.h"
#include "../wire/scripts.h"

#include <array>
#include <format>
#include <fstream>
#include <print>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

namespace convert {

namespace {

std::stack<std::vector<uint8_t>> one_stack_end_operations(
	std::vector<uint8_t>& bufferScripts,
	std::vector<uint8_t>& operand1,
	std::stack<std::vector<uint8_t>>& stack_for_RPN)
{
	operand1 = stack_for_RPN.top(); stack_for_RPN.pop();
	bufferScripts.insert(bufferScripts.end(), operand1.begin(), operand1.end());
	std::format_to(std::back_inserter(bufferScripts), "$END\n!\n");
	operand1.clear();
	return stack_for_RPN;
}

std::stack<std::vector<uint8_t>> two_stack_end_operations(
	std::vector<uint8_t>& bufferScripts,
	std::vector<uint8_t>& operand1,
	std::vector<uint8_t>& operand2,
	std::stack<std::vector<uint8_t>>& stack_for_RPN)
{
	operand2 = stack_for_RPN.top(); stack_for_RPN.pop();
	operand1 = stack_for_RPN.top(); stack_for_RPN.pop();
	bufferScripts.insert(bufferScripts.end(), operand1.begin(), operand1.end());
	bufferScripts.insert(bufferScripts.end(), operand2.begin(), operand2.end());
	std::format_to(std::back_inserter(bufferScripts), "$END\n!\n");
	operand1.clear();
	operand2.clear();
	return stack_for_RPN;
}

std::stack<std::vector<uint8_t>> stack_operations(
	std::vector<uint8_t>& operand1,
	std::vector<uint8_t>& operand2,
	std::stack<std::vector<uint8_t>>& stack_for_RPN,
	const std::string& script_name)
{
	operand2 = stack_for_RPN.top(); stack_for_RPN.pop();
	operand1 = stack_for_RPN.top(); stack_for_RPN.pop();
	operand1.insert(operand1.end(), script_name.begin(), script_name.end());
	operand1.insert(operand1.end(), operand2.begin(), operand2.end());
	stack_for_RPN.push(operand1);
	operand1.clear();
	operand2.clear();
	return stack_for_RPN;
}

std::stack<std::vector<uint8_t>> stack_operations_NF(
	std::vector<uint8_t>& operand2,
	std::stack<std::vector<uint8_t>>& stack_for_RPN,
	const std::string& script_name)
{
	operand2 = stack_for_RPN.top(); stack_for_RPN.pop();
	operand2.insert(operand2.begin(), script_name.begin(), script_name.end());
	stack_for_RPN.push(operand2);
	operand2.clear();
	return stack_for_RPN;
}

std::stack<std::vector<uint8_t>> stack_operations_bracket(
	std::vector<uint8_t>& operand1,
	std::vector<uint8_t>& operand2,
	std::stack<std::vector<uint8_t>>& stack_for_RPN,
	const std::string& script_name,
	const std::string& bracket_open,
	const std::string& bracket_close)
{
	operand2 = stack_for_RPN.top(); stack_for_RPN.pop();
	operand1 = stack_for_RPN.top(); stack_for_RPN.pop();
	operand1.insert(operand1.begin(), bracket_open.begin(), bracket_open.end());
	operand1.insert(operand1.end(), script_name.begin(), script_name.end());
	operand1.insert(operand1.end(), operand2.begin(), operand2.end());
	operand1.insert(operand1.end(), bracket_close.begin(), bracket_close.end());
	stack_for_RPN.push(operand1);
	operand1.clear();
	operand2.clear();
	return stack_for_RPN;
}

void convert_opn(
	std::vector<uint8_t>& bufferScripts,
	std::vector<uint8_t>& operand1,
	std::vector<uint8_t>& operand2,
	std::stack<std::vector<uint8_t>>& stack_for_RPN,
	uint32_t logicOperator,
	uint32_t scriptNumber)
{
	switch (scriptNumber)
	{
	case 1:
		switch (logicOperator)
		{
		case 3: std::format_to(std::back_inserter(bufferScripts), "$nF\n"); break;
		case 4: one_stack_end_operations(bufferScripts, operand1, stack_for_RPN); break;
		}
		break;
	default:
		switch (logicOperator)
		{
		case 1:
			if (stack_for_RPN.size() > 2)
				stack_operations_bracket(operand1, operand2, stack_for_RPN, "$&\n", "$(\n", "$)\n");
			else
			{
				if (stack_for_RPN.size() == 1)
					throw std::logic_error("There is only one element in stack_for_RPN");
				stack_operations(operand1, operand2, stack_for_RPN, "$&\n");
			}
			break;
		case 2:
			if (stack_for_RPN.size() > 2)
				stack_operations_bracket(operand1, operand2, stack_for_RPN, "$|\n", "$(\n", "$)\n");
			else
			{
				if (stack_for_RPN.size() == 1)
					throw std::logic_error("There is only one element in stack_for_RPN");
				stack_operations(operand1, operand2, stack_for_RPN, "$|\n");
			}
			break;
		case 3: stack_operations_NF(operand2, stack_for_RPN, "$nF\n"); break;
		case 4:
			if (stack_for_RPN.size() > 1)
				two_stack_end_operations(bufferScripts, operand1, operand2, stack_for_RPN);
			else
				one_stack_end_operations(bufferScripts, operand1, stack_for_RPN);
			break;
		}
	}
}

void get_script_size(std::vector<uint8_t>& bufferScripts,
                     uint32_t cur_script_num,
                     std::string_view script_name)
{
	std::vector<uint8_t> cap_scripts;
	std::format_to(std::back_inserter(cap_scripts), "script \"{}{}\" size {}\n",
	    cur_script_num, script_name, bufferScripts.size());
	bufferScripts.insert(bufferScripts.begin(), cap_scripts.begin(), cap_scripts.end());
	std::format_to(std::back_inserter(bufferScripts), "\r\n");
}

} // namespace

void mis_scripts(const std::filesystem::path& mis_folder,
                 std::string_view stem_name,
                 std::string_view data)
{
	std::ofstream f(mis_folder / "scripts2", std::ios::binary);
	if (!f)
	{
		std::println(stderr, "\033[31m[Error]\033[0m cannot write scripts2");
		return;
	}

	std::vector<uint8_t> bufferScripts;
	std::vector<uint8_t> operand1, operand2;
	std::stack<std::vector<uint8_t>> stack_for_RPN;

	uint32_t cur_script_num = 1;

	std::array<std::string, MAX_NUM_OF_ARGS_IN_INSTRUCTION> instruction_args;
	std::array<int, MAX_NUM_OF_ARGS_IN_INSTRUCTION> instr_args_num;

	for (size_t curOffset = 4; curOffset + sizeof(scripts1) < data.size();)
	{
		std::string script_name;

		const scripts1& struct_scripts = *(scripts1*)(data.data() + curOffset);
		curOffset += sizeof(scripts1);

		uint32_t n = 0;
		uint32_t script_num_for_OPN = 0;

		for (size_t i = 0; i < (struct_scripts.size_of_script - sizeof(scripts2)) / sizeof(scripts2); i++)
		{
			const scripts2& scripts = *(scripts2*)(data.data() + curOffset);
			curOffset += sizeof(scripts2);
			uint8_t logicOperator = 0;

			if (scripts.num < 0) // negative int = numeric argument, positive = opcode
			{
				instruction_args[n] = reverse_num(scripts.num & 0x7FFFFFFF);
				instr_args_num[n] = scripts.num & 0x7FFFFFFF;
				n++;
			}
			else
			{
				n = 0;
				switch (scripts.num)
				{
				case bufferAND: logicOperator = 1; break;
				case bufferOR:  logicOperator = 2; break;
				case bufferNF:  logicOperator = 3; break;

				case bufferUG: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$ug\n$UMSK\n#{}\n$ug1\n$Grp\n#{}\n$sp_8\n$CABE\n#{}\n$sp_9\n$N1\n#{}\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3]);
					script_num_for_OPN += 1; break;

				case bufferUP: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$up\n$UMSK\n#{}\n$up1\n$Plr\n#{}\n$sp_b\n$CABE\n#{}\n$sp_c\n$N1\n#{}\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3]);
					script_num_for_OPN += 1; break;

				case bufferUGL: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$ugl\n$CABE\n#{}\n$N1\n#{}\n$umx0\n$Grp\n#{}\n$ugl1\n$L\n#{}\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3]);
					script_num_for_OPN += 1; break;

				case bufferUPL: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$upl\n$CABE\n#{}\n$N1\n#{}\n$umx2\n$Plr\n#{}\n$upl1\n$L\n#{}\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3]);
					script_num_for_OPN += 1; break;

				case bufferPKP: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$pkp\n$Plr\n#{}\n$pkp1\n$CABE\n#{}\n$sp_d\n$N1\n#{}\n$sp_e\n$PT\n#{}\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3]);
					script_num_for_OPN += 1; break;

				case bufferPKF: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$pkf\n$Plr\n#{}\n$pkf1\n$CABE\n#{}\n$sp_f\n$N1\n#{}\n$pkf2\n$PT\n#{}\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3]);
					script_num_for_OPN += 1; break;

				case bufferTE: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$te\n$Tr\n#{}\n"
					, instruction_args[0]);
					script_name += std::format("_alarm-{}", instr_args_num[0]);
					script_num_for_OPN += 1; break;

				case bufferCD: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$cd\n$CABE\n#{}\n$sp_5\n$T\n#{}\n"
					, instruction_args[0], instruction_args[1]);
					script_num_for_OPN += 1; break;

				case bufferTMS: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$tms\n$CAB\n#{}\n$sp_6\n$T\n#{}\n"
					, instruction_args[0], instruction_args[1]);
					script_num_for_OPN += 1; break;

				case bufferUGLper: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$ugl%\n$CABE\n#{}\n$N1\n#{}\n$umx1\n$Grp\n#{}\n$ugl1\n$L\n#{}\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3]);
					script_num_for_OPN += 1; break;

				case bufferUPLper: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$upl%\n$CABE\n#{}\n$N1\n#{}\n$umx3\n$Plr\n#{}\n$upl1\n$L\n#{}\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3]);
					script_num_for_OPN += 1; break;

				case bufferUGper: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$ug%u\n$UMSK\n#{}\n$umx4\n$Grp\n#{}\n$sp_7\n$CABE\n#{}\n$sp_k\n$N1\n#{}\n$ugut1\n$UMSK\n#{}\n$ug%u1\n$Grp\n#{}\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3], instruction_args[4], instruction_args[5]);
					script_num_for_OPN += 1; break;

				case bufferUPper: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$up%\n$UMSK\n#{}\n$up%u1\n$Plr\n#{}\n$sp_a\n$CABE\n#{}\n$sp_k\n$N1\n#{}\n$ugut1\n$UMSK\n#{}\n$up%u2\n$Plr\n#{}\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3], instruction_args[4], instruction_args[5]);
					script_num_for_OPN += 1; break;

				case bufferAIGB: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$aigb\n$Grp\n#{}\n$aigb1\n$AI\n#{}\n"
					, instruction_args[0], instruction_args[1]);
					script_num_for_OPN += 1; break;

				case bufferAIZ1: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$aiz1\n$Grp\n#{}\n$aiz_1\n$L\n#{}\n"
					, instruction_args[0], instruction_args[1]);
					script_num_for_OPN += 1; break;

				case bufferAIZ2: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$aiz2\n$Grp\n#{}\n$aiz_2\n$L\n#{}\n"
					, instruction_args[0], instruction_args[1]);
					script_num_for_OPN += 1; break;

				case bufferAIG1: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$aig1\n$Grp\n#{}\n$aig_1\n$Grp\n#{}\n"
					, instruction_args[0], instruction_args[1]);
					script_num_for_OPN += 1; break;

				case bufferAIG2: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$aig2\n$Grp\n#{}\n$aig_2\n$Grp\n#{}\n"
					, instruction_args[0], instruction_args[1]);
					script_num_for_OPN += 1; break;

				case bufferOID: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$oid\n$Obj\n#{}\n$oid1\n"
					, instruction_args[0]);
					script_num_for_OPN += 1; break;

				case bufferGWATA: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$gwata\n$Grp\n#{}\n$gwata1\n$CAB\n#{}\n$T\n#{}\n$gwata2\n"
					, instruction_args[0], instruction_args[1], instruction_args[2]);
					script_num_for_OPN += 1; break;

				case bufferVIC: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$vic\n$Lvar\n#{}\n$CABE\n#{}\n$Rvar\n#{}\n"
					, instruction_args[0], instruction_args[1], instruction_args[2]);
					script_num_for_OPN += 1; break;

				case bufferMIST: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$mist\n");
					script_num_for_OPN += 1; break;

				case bufferEND: logicOperator = 4; break;

				case bufferSPPL: std::format_to(std::back_inserter(bufferScripts), "$sppl\n$I\n#{}\n$spc\n$PT\n#{}\n$sppl1\n$P\n#{}\n$sppl2\n$L\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3]);
					script_name += std::format("_snd-{}-plane-{}-{}-zone-{}"
						, instr_args_num[0]
						, (instr_args_num[1] == 0) ? "bmb" : (instr_args_num[1] == 1) ? "int" : (instr_args_num[1] == 2) ? "tr" : "crg"
						, (instr_args_num[2] == 0) ? "pl" : (instr_args_num[2] == 1) ? "en" : (instr_args_num[2] == 2) ? "al" : "neu"
						, instr_args_num[3]);
					break;

				case bufferETC: std::format_to(std::back_inserter(bufferScripts), "$etc\n$\\\\n\n");
					break;

				case bufferSTRT: std::format_to(std::back_inserter(bufferScripts), "$strt\n$Tr\n#{}\n$strt1\n$T\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1]);
					script_name += std::format("_set-al-{}", instr_args_num[0]);
					break;

				case bufferSTPT: std::format_to(std::back_inserter(bufferScripts), "$stpt\n$Tr\n#{}\n$\\\\n\n"
					, instruction_args[0]);
					break;

				case bufferMSTL: std::format_to(std::back_inserter(bufferScripts), "$mstl\n$L\n#{}\n$\\\\n\n"
					, instruction_args[0]);
					break;

				case bufferSP: std::format_to(std::back_inserter(bufferScripts), "$sp\n$phrs\n#{}\n$\\\\n\n"
					, instruction_args[0]);
					break;

				case bufferSCD: std::format_to(std::back_inserter(bufferScripts), "$scd\n$T\n#{}\n$\\\\n\n"
					, instruction_args[0]);
					break;

				case bufferSNM: std::format_to(std::back_inserter(bufferScripts), "$snm\n$mission\n#{}\n$\\\\n\n"
					, instruction_args[0]);
					break;

				case bufferTM: std::format_to(std::back_inserter(bufferScripts), "$tm\n$F\n#{}\n$\\\\n\n"
					, instruction_args[0]);
					script_name += instr_args_num[0] == 1 ? "_win" : instr_args_num[0] == 2 ? "_lose" : "_draw";
					break;

				case bufferSGB: std::format_to(std::back_inserter(bufferScripts), "$sgb\n$G\n#{}\n$sgb1\n$AI\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1]);
					break;

				case bufferSGL1: std::format_to(std::back_inserter(bufferScripts), "$sgl1\n$G\n#{}\n$sgl11\n$L\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1]);
					break;

				case bufferSGL2: std::format_to(std::back_inserter(bufferScripts), "$sgl2\n$G\n#{}\n$sgl21\n$L\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1]);
					break;

				case bufferSGG1: std::format_to(std::back_inserter(bufferScripts), "$sgg1\n$G\n#{}\n$sgg11\n$G\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1]);
					break;

				case bufferSGG2: std::format_to(std::back_inserter(bufferScripts), "$sgg2\n$G\n#{}\n$sgg21\n$G\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1]);
					break;

				case bufferAPP: std::format_to(std::back_inserter(bufferScripts), "$app\n$P\n#{}\n$spc\n$I\n#{}\n$spc\n$PT\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1], instruction_args[2]);
					break;

				case bufferAFP: std::format_to(std::back_inserter(bufferScripts), "$afp\n$P\n#{}\n$spc\n$I\n#{}\n$afp1\n$PT\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1], instruction_args[2]);
					break;

				case bufferRU: std::format_to(std::back_inserter(bufferScripts), "$ru\n$G\n#{}\n$ru1\n$flg\n#{}\n$ru2\n$L\n#{}\n$ru3\n$T\n#{}\n$ru4\n$I\n#{}\n$ru5\n$I\n#{}\n$ru5\n$I\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3], instruction_args[4], instruction_args[5], instruction_args[6]);
					break;

				case bufferSRFS: std::format_to(std::back_inserter(bufferScripts), "$srfs\n$P\n#{}\n$srfs0\n$resv\n#{}\n$srfs1\n$flg\n#{}\n$srfs2\n$L\n#{}\n$srfs3\n$T\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3], instruction_args[4]);
					script_name += std::format("_reinf-{}-{}-{}"
						, (instr_args_num[0] == 0) ? "pl" : (instr_args_num[0] == 1) ? "en" : (instr_args_num[0] == 2) ? "al" : "neu"
						, (char)('A' + instr_args_num[2])
						, std::string{ instruction_args[3].rbegin(), instruction_args[3].rend() });
					break;

				case bufferSPTO: std::format_to(std::back_inserter(bufferScripts), "$spto\n$I\n#{}\n$spc\n$PT\n#{}\n$spto1\n$P\n#{}\n$spto2\n$Obj\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3]);
					script_name += std::format("_snd-{}-plane-{}-{}-{}"
						, instr_args_num[0]
						, (instr_args_num[1] == 0) ? "bmb" : (instr_args_num[1] == 1) ? "int" : (instr_args_num[1] == 2) ? "tr" : "crg"
						, (instr_args_num[2] == 0) ? "pl" : (instr_args_num[2] == 1) ? "en" : (instr_args_num[2] == 2) ? "al" : "neu"
						, instr_args_num[3]);
					break;

				case bufferSAT: std::format_to(std::back_inserter(bufferScripts), "$sat\n$\\\\n\n");
					break;

				case bufferARPO: std::format_to(std::back_inserter(bufferScripts), "$arpo\n$Obj\n#{}\n$\\\\n\n"
					, instruction_args[0]);
					break;

				case bufferASPO: std::format_to(std::back_inserter(bufferScripts), "$aspo\n$Obj\n#{}\n$\\\\n\n"
					, instruction_args[0]);
					break;

				case bufferSPPA: std::format_to(std::back_inserter(bufferScripts), "$sppa\n$I\n#{}\n$spc\n$PT\n#{}\n$sppa1\n$P\n#{}\n$sppa2\n$\\\\n\n"
					, instruction_args[0], instruction_args[1], instruction_args[2]);
					script_name += std::format("_snd-{}-plane-route-{}-{}"
						, instr_args_num[0]
						, (instr_args_num[1] == 0) ? "bmb" : (instr_args_num[1] == 1) ? "int" : (instr_args_num[1] == 2) ? "tr" : "crg"
						, (instr_args_num[2] == 0) ? "pl" : (instr_args_num[2] == 1) ? "en" : (instr_args_num[2] == 2) ? "al" : "neu");
					break;

				case bufferLCCV: std::format_to(std::back_inserter(bufferScripts), "$lccv\n$Lvar\n#{}\n$lccv1\n$Rvar\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1]);
					break;

				case bufferMO: std::format_to(std::back_inserter(bufferScripts), "$mo\n$Lvar\n#{}\n$spc\n$Math\n#{}\n$spc\n$Rvar\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1], instruction_args[2]);
					break;

				case bufferSRES: std::format_to(std::back_inserter(bufferScripts), "$sres\n$resv\n#{}\n$sres1\n$resv\n#{}\n$sres2\n$resv\n#{}\n$sres3\n$T\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3]);
					break;

				case bufferFRES: std::format_to(std::back_inserter(bufferScripts), "$fres\n$Fmask\n#{}\n$fres0\n$resv\n#{}\n$fres1\n$resv\n#{}\n$fres2\n$resv\n#{}\n$fres3\n$T\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1], instruction_args[2], instruction_args[3], instruction_args[4]);
					break;

				case bufferPFF: std::format_to(std::back_inserter(bufferScripts), "$pff\n$Fmask\n#{}\n$spc\n$I\n#{}\n$spc\n$PT\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1], instruction_args[2]);
					break;

				case bufferFPFF: std::format_to(std::back_inserter(bufferScripts), "$fpff\n$Fmask\n#{}\n$spc\n$I\n#{}\n$fpff1\n$PT\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1], instruction_args[2]);
					break;

				case bufferMFF: std::format_to(std::back_inserter(bufferScripts), "$mff\n$Fmask\n#{}\n$spc\n$phrs\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1]);
					break;

				case bufferSPPO: std::format_to(std::back_inserter(bufferScripts), "$sppo\n$phrs\n#{}\n$sppo1\n$Obj\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1]);
					break;

				case bufferDGP: std::format_to(std::back_inserter(bufferScripts), "$dgp\n$G\n#{}\n$dgp1\n$P\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1]);
					break;

				case bufferFVKGZ: std::format_to(std::back_inserter(bufferScripts), "$fvkgz\n$G\n#{}\n$fvkgz1\n$L\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1]);
					break;

				case bufferFVKGO: std::format_to(std::back_inserter(bufferScripts), "$fvkgo\n$G\n#{}\n$fvkgo1\n$Obj\n#{}\n$\\\\n\n"
					, instruction_args[0], instruction_args[1]);
					script_name += std::format("_kat-{}", instr_args_num[0]);
					break;

				case 0x7FFFFFFF: std::format_to(std::back_inserter(bufferScripts), "$END\n");
					break;

				default:
					std::format_to(std::back_inserter(bufferScripts), "unknown script {}\n", scripts.num);
					std::println("\033[31m[Error]\033[0m Found unknown script in {}: {}.", stem_name, scripts.num);
					break;
				}
			}

			try
			{
				convert_opn(bufferScripts, operand1, operand2, stack_for_RPN, logicOperator, script_num_for_OPN);
			}
			catch (std::logic_error e)
			{
				std::println("\033[31m[Error]\033[0m Got exception for {}: {}", scripts.num, e.what());
			}
		}

		script_name = script_name.substr(0, 61); // max script name length is 63 symbols - 2 for num
		get_script_size(bufferScripts, cur_script_num, script_name);
		f.write((char*)bufferScripts.data(), static_cast<std::streamsize>(bufferScripts.size()));

		bufferScripts.clear();
		++cur_script_num;
	}
}

} // namespace convert
