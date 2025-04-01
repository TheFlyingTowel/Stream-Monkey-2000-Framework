#include "pch.h"

namespace SM2K
{

	CmdTable Interpreter::s_cmdTable;

#define EatArgs(tokenTypes)\
						eat(tokenTypes);\
						while (m_current.type)\
						{\
							m_currentArgList.emplace_back(m_current);\
							eat(tokenTypes);\
						}\
						++m_tpos;



	Interpreter::Interpreter(const _REGENT& _reg, const string& _data, RequestType _req)
		:m_unTokenizedData{_data}, m_requestType{_req},r_registry{*_reg.registry}, r_entity{_reg.entity},
		TokenizedData{m_TokenizedData}, UnTokenizedData{m_unTokenizedData}
	{

		_tokenize();
		try {


			_interprete();
		}
		catch (Error e) {
			std::cout << e << std::endl;
		}

	}

	void Interpreter::_tokenize()
	{
		vector(string) splitData = Split(m_unTokenizedData, ";");
		char currStrOp = '\0';
		for (auto i = splitData.begin(), e = splitData.end(); i != e; ++i)
		{
			cstring word;
			vector(string) cmd = Split(*i, " ");
			for(int j = 0; j < cmd.size(); ++j)
			{
				word = cmd[j].c_str();
				bool op;
				if (s_cmdTable.contains(word) || ( op = (word[0] == '"' || word[0] == '\'')))
				{
					// Find out if this is a string
					(op) ? m_TokenizedData.emplace_back(Token(STR_OPEN, string( word[0], '\0' ).c_str())) : m_TokenizedData.emplace_back(s_cmdTable[word]);
					switch (m_TokenizedData.back().type)
					{
						case OP: // Format string data
						{
							currStrOp = m_TokenizedData.back().data[0];
							if (cmd[j].length() < 2) continue;
							switch (currStrOp)
							{
							case '\"':
							case '\'':
							{
								string str = "";
								int n = 1;
								while (word[n] != currStrOp)
								{
									str += word[n++];
									if (cmd[j].length() <= n)
									{
										word = cmd[++j].c_str();
										str += " ";
										n = 0;
									}
								}
								m_TokenizedData.emplace_back(Token(STRING, str.c_str()));
								m_TokenizedData.emplace_back(Token(STR_CLOSE, string(currStrOp, '\0').c_str())); // Add closing string token

							}
							break;
							}

						}
						break;

					}
				}
				else m_TokenizedData.emplace_back(Token(NAME, word));
			}
			m_TokenizedData.emplace_back(s_cmdTable["\0"]); // Sepperate the cmds.
		}
	}

	void Interpreter::_interprete()
	{
		while (m_tpos < m_TokenizedData.size())
		{
			eat(CMD);
			m_currentArgList.clear();
			switch (m_current.hash)
			{
			
			TOKEN_CASE(_ECHO)
			{
				EatArgs(NAME | STRING | STR_OPEN | STR_CLOSE);

				// TODO: Create Echo cmd.
			
			}
			break;
			
			TOKEN_CASE(_OPEN)
			{
				EatArgs(NAME);

				// TODO: Create Open cmd.
				r_registry.emplace<OpenStream>(r_entity);
				r_registry.patch<Core>(r_registry.ctx().get<_Entity>());
			}
			break;
			
			TOKEN_CASE(_CLOSE)
			{
				EatArgs(NAME);

				// TODO: Create Close cmd.
			}
			break;
			
			TOKEN_CASE(_START)
			{
				EatArgs(NAME);

				// TODO: Create Start cmd.
			}
			break;
			
			TOKEN_CASE(_STOP)
			{
				EatArgs(NAME);

				// TODO: Create Echo cmd.
			}
			break;
			
			TOKEN_CASE(_QUIT)
			{
				EatArgs(EOT);

				// TODO: Create Quit cmd.
			}
			break;

			// TODO: Add Command builers here ^^^
			}
		}
		m_TokenizedData.clear();

	}

	void Interpreter::eat(TokenType _type)
	{
		if (m_TokenizedData[m_tpos].type && !(m_TokenizedData[m_tpos].type & _type))
		{
			switch (_type)
			{
			case SM2K::CMD:
			{
				string tmp = "Expected a command, but got  -> ";

				throw Error(SYNTAX, tmp + m_TokenizedData[m_tpos].data);
			}
			break;
			case SM2K::STRING:
			{
				string tmp = "Expected string text, but got  -> ";
				throw  Error(SYNTAX, tmp + m_TokenizedData[m_tpos].data + "\"");
			}
				break;
			case SM2K::INT:
			{
				string tmp = "Expected an integer, but got  -> ";
				throw Error(SYNTAX, tmp + m_TokenizedData[m_tpos].data);
			}
				break;
			case SM2K::FLOAT:
			{
				string tmp = "Expected a floating point number, but got  -> ";
				throw  Error(SYNTAX, tmp + m_TokenizedData[m_tpos].data);
			}
				break;
			case SM2K::DOUBLE:
				{
				string tmp = "Expected a decimal number, but got  -> ";
				throw  Error(SYNTAX, tmp + m_TokenizedData[m_tpos].data);
				}
				break;
			case SM2K::STR_OPEN:
			{
				string tmp = "Expected a ' or \" but got  -> ";
				throw  Error(SYNTAX, tmp + m_TokenizedData[m_tpos].data);
			}
				break;
			case SM2K::STR_CLOSE:
			{
				string tmp = "Expected a ' or \" but got  -> ";
				throw  Error(SYNTAX, tmp + m_TokenizedData[m_tpos].data);
			}
				break;
			default:
			{
				string tmp = "Unexpected syntax, got  -> ";
				throw  Error(SYNTAX, tmp + m_TokenizedData[m_tpos].data);
			}
				break;
			}
		}
		else 
		{
			m_current = m_TokenizedData[m_tpos];
			
			++m_tpos;
		}
	}

};