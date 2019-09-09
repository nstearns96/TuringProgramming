#include "Optimization.h"

Instruction::Instruction(int code, std::vector<std::string> args)
{
	instructionCode = code;
	arguments = args;
}

int Instruction::getCode() const
{
	return instructionCode;
}

std::vector<std::string> Instruction::getArgs() const
{
	return arguments;
}

bool validOptimization(const std::vector<Instruction> &ins, std::string arg)
{
	if (ins.size() == 0)
		return true;
	switch (ins[ins.size() - 1].getCode())
	{
		case opAcc:
		{
			return false;
			break;
		}
		case opRej:
		{
			return false;
			break;
		}
		case opGoto:
		{
			return false;
			break;
		}
		case opIfGoto:
		{
			if (arg == "0" || arg == "1" || arg == "4" || arg == "2")
			{
				return true;
			}
			else
			{
				for (int i = 0; i < ins.size(); ++i)
				{
					if (ins[i].getCode() == std::stoi(arg))
					{
						return false;
					}
				}
				return true;
			}
			break;
		}
		case opWrite:
		{
			if (arg == "0" || arg == "1" || arg == "4" || arg == "5")
			{
				return true;
			}
			else
			{
				return false;
			}
			break;
		}
		case opMove:
		{
			if (arg == "0" || arg == "1" || arg == "4")
			{
				return true;
			}
			else
			{
				return false;
			}
			break;
		}
	}
}

//TODO: Needs major refactoring and precomputations
Table getOptimizedStates(const std::vector<Instruction> &ins, const std::vector<char> &alphabet, unsigned int numStates)
{
	Table result = { 0, {}, {}, {} };
	if (ins.size() > 0)
	{
		switch (ins[0].getCode())
		{
			case opAcc:
			{
				result.numStates = 1;
				for (int c = 0; c < alphabet.size(); ++c)
				{
					result.write.push_back(alphabet[c]);
					result.move.push_back('r');
					result.nextState.push_back("a");
				}

				return result;
				break;
			}
			case opRej:
			{
				result.numStates = 1;
				for (int c = 0; c < alphabet.size(); ++c)
				{
					result.write.push_back(alphabet[c]);
					result.move.push_back('r');
					result.nextState.push_back("r");
				}

				return result;
				break;
			}
			case opIfGoto:
			{
				std::vector<char> alphabetSplit;
				int ifGotoEnd = 0;
				for (; (ifGotoEnd < ins.size()) && (ins[ifGotoEnd].getCode() == opIfGoto); ++ifGotoEnd)
				{
					alphabetSplit.push_back(alphabet[std::stoi(ins[ifGotoEnd].getArgs()[0])]);
				}

				if (ins.size() == ifGotoEnd)
				{
					result.numStates = 2;
					for (int c = 0; c < alphabet.size(); ++c)
					{
						result.write.push_back(alphabet[c]);
						result.move.push_back('r');
						result.nextState.push_back(std::to_string(numStates+1));
					}

					//TODO: Look-ahead optimization
					for (int c = 0; c < alphabet.size(); ++c)
					{
						result.write.push_back(alphabet[c]);
						result.move.push_back('l');
						if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
						{
							result.nextState.push_back(std::to_string(numStates + 2));
						}
						else
						{
							for (int i = 0; i < ifGotoEnd; ++i)
							{
								if (std::stoi(ins[i].getArgs()[1]) == c)
								{
									result.nextState.push_back("line" + ins[i].getArgs()[1]);
									break;
								}
							}
						}
					}

					return result;
				}
				else
				{
					switch (ins[ifGotoEnd].getCode())
					{
						case opAcc:
						{
							result.numStates = 2;
							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[c]);
								result.move.push_back('r');
								if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
								{
									result.nextState.push_back("a");
								}
								else
								{
									for (int i = 0; i < ifGotoEnd; ++i)
									{
										if (std::stoi(ins[i].getArgs()[0]) == c)
										{
											result.nextState.push_back(std::to_string(numStates + 1));
											break;
										}
									}
								}
							}

							//TODO: Look-ahead optimization
							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[c]);
								result.move.push_back('l');
								if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
								{
									result.nextState.push_back("a");
								}
								else
								{
									for (int i = 0; i < ifGotoEnd; ++i)
									{
										if (std::stoi(ins[i].getArgs()[0]) == c)
										{
											result.nextState.push_back("line" + ins[i].getArgs()[1]);
											break;
										}
									}
								}
							}

							return result;
							break;
						}
						case opRej:
						{
							result.numStates = 2;
							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[c]);
								result.move.push_back('r');
								if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
								{
									result.nextState.push_back("r");
								}
								else
								{
									for (int i = 0; i < ifGotoEnd; ++i)
									{
										if (std::stoi(ins[i].getArgs()[0]) == c)
										{
											result.nextState.push_back(std::to_string(numStates + 1));
											break;
										}
									}
								}
							}

							//TODO: Look-ahead optimization
							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[c]);
								result.move.push_back('l');
								if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
								{
									result.nextState.push_back("r");
								}
								else
								{
									for (int i = 0; i < ifGotoEnd; ++i)
									{
										if (std::stoi(ins[i].getArgs()[0]) == c)
										{
											result.nextState.push_back("line" + ins[i].getArgs()[1]);
											break;
										}
									}
								}
							}

							return result;
							break;
						}
						case opWrite:
						{
							if (ins.size() == ifGotoEnd + 1)
							{
								result.numStates = 2;
								for (int c = 0; c < alphabet.size(); ++c)
								{
									if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
									{
										result.write.push_back(alphabet[std::stoi(ins[ifGotoEnd].getArgs()[0])]);
									}
									else
									{
										result.write.push_back(alphabet[c]);
									}
									result.move.push_back('r');
									result.nextState.push_back(std::to_string(numStates + 1));
								}

								//TODO: Look-ahead optimization
								for (int c = 0; c < alphabet.size(); ++c)
								{
									result.write.push_back(alphabet[c]);
									result.move.push_back('l');
									if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
									{
										result.nextState.push_back(std::to_string(numStates + 2));
									}
									else
									{
										for (int i = 0; i < ifGotoEnd; ++i)
										{
											if (std::stoi(ins[i].getArgs()[0]) == c)
											{
												result.nextState.push_back("line" + ins[i].getArgs()[1]);
												break;
											}
										}
									}
								}

								return result;
								break;
							}
							else
							{
								switch (ins[ifGotoEnd].getCode())
								{
									case opAcc:
									{
										result.numStates = 2;
										for (int c = 0; c < alphabet.size(); ++c)
										{
											if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
											{
												result.write.push_back(alphabet[std::stoi(ins[ifGotoEnd].getArgs()[0])]);
											}
											else
											{
												result.write.push_back(alphabet[c]);
											}
											result.move.push_back('r');
											result.nextState.push_back(std::to_string(numStates + 1));
										}

										//TODO: Look-ahead optimization
										for (int c = 0; c < alphabet.size(); ++c)
										{
											result.write.push_back(alphabet[c]);
											result.move.push_back('l');
											if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
											{
												result.nextState.push_back("a");
											}
											else
											{
												for (int i = 0; i < ifGotoEnd; ++i)
												{
													if (std::stoi(ins[i].getArgs()[0]) == c)
													{
														result.nextState.push_back("line" + ins[i].getArgs()[1]);
														break;
													}
												}
											}
										}

										return result;
										break;
									}
									case opRej:
									{
										result.numStates = 2;
										for (int c = 0; c < alphabet.size(); ++c)
										{
											if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
											{
												result.write.push_back(alphabet[std::stoi(ins[ifGotoEnd].getArgs()[0])]);
											}
											else
											{
												result.write.push_back(alphabet[c]);
											}
											result.move.push_back('r');
											result.nextState.push_back(std::to_string(numStates + 1));
										}

										//TODO: Look-ahead optimization
										for (int c = 0; c < alphabet.size(); ++c)
										{
											result.write.push_back(alphabet[c]);
											result.move.push_back('l');
											if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
											{
												result.nextState.push_back("r");
											}
											else
											{
												for (int i = 0; i < ifGotoEnd; ++i)
												{
													if (std::stoi(ins[i].getArgs()[0]) == c)
													{
														result.nextState.push_back("line" + ins[i].getArgs()[1]);
														break;
													}
												}
											}
										}

										return result;
										break;
									}
									case opIfGoto:
									{
										std::vector<char> secondAlphabetSplit;
										for (int i = ifGotoEnd + 1; i < ins.size(); ++i)
										{
											secondAlphabetSplit.push_back(alphabet[std::stoi(ins[i].getArgs()[0])]);
										}

										result.numStates = 2;
										for (int c = 0; c < alphabet.size(); ++c)
										{
											if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
											{
												result.write.push_back(alphabet[std::stoi(ins[ifGotoEnd].getArgs()[0])]);
											}
											else
											{
												result.write.push_back(alphabet[c]);
											}
											result.move.push_back('r');
											result.nextState.push_back(std::to_string(numStates + 1));
										}

										//TODO: Look-ahead optimization
										for (int c = 0; c < alphabet.size(); ++c)
										{
											result.write.push_back(alphabet[c]);
											result.move.push_back('l');
											if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
											{
												if (std::find(secondAlphabetSplit.begin(), secondAlphabetSplit.end(), alphabet[c]) == secondAlphabetSplit.end())
												{
													result.nextState.push_back(std::to_string(numStates + 2));
												}
												else
												{
													for (int i = ifGotoEnd + 1; i < ins.size(); ++i)
													{
														if (std::stoi(ins[i].getArgs()[0]) == c)
														{
															result.nextState.push_back("line" + ins[i].getArgs()[1]);
															break;
														}
													}
												}
											}
											else
											{
												for (int i = 0; i < ifGotoEnd; ++i)
												{
													if (std::stoi(ins[i].getArgs()[0]) == c)
													{
														result.nextState.push_back("line" + ins[i].getArgs()[1]);
														break;
													}
												}
											}
										}

										return result;
										break;
									}
									case opGoto:
									{
										result.numStates = 2;
										for (int c = 0; c < alphabet.size(); ++c)
										{
											if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
											{
												result.write.push_back(alphabet[std::stoi(ins[ifGotoEnd].getArgs()[0])]);
											}
											else
											{
												result.write.push_back(alphabet[c]);
											}
											result.move.push_back('r');
											result.nextState.push_back(std::to_string(numStates + 1));
										}

										//TODO: Look-ahead optimization
										for (int c = 0; c < alphabet.size(); ++c)
										{
											result.write.push_back(alphabet[c]);
											result.move.push_back('l');
											if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
											{
												result.nextState.push_back("line" + ins[ifGotoEnd + 1].getArgs()[0]);
											}
											else
											{
												for (int i = 0; i < ifGotoEnd; ++i)
												{
													if (std::stoi(ins[i].getArgs()[0]) == c)
													{
														result.nextState.push_back("line" + ins[i].getArgs()[1]);
														break;
													}
												}
											}
										}

										return result;
										break;
									}
									case opMove:
									{
										if (ins.size() == ifGotoEnd + 2)
										{
											result.numStates = 2;
											for (int c = 0; c < alphabet.size(); ++c)
											{
												if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
												{
													result.write.push_back(alphabet[std::stoi(ins[ifGotoEnd].getArgs()[0])]);
													result.move.push_back(ins[ifGotoEnd + 1].getArgs()[0][0]);
													result.nextState.push_back(std::to_string(numStates + 2));
												}
												else
												{
													result.write.push_back(alphabet[c]);
													result.write.push_back('r');
													result.nextState.push_back(std::to_string(numStates + 1));
												}
											}

											//TODO: Look-ahead optimization
											for (int c = 0; c < alphabet.size(); ++c)
											{
												result.write.push_back(alphabet[c]);
												result.move.push_back('l');
												if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
												{
													result.nextState.push_back(std::to_string(numStates + 2));
												}
												else
												{
													for (int i = 0; i < ifGotoEnd; ++i)
													{
														if (std::stoi(ins[i].getArgs()[0]) == c)
														{
															result.nextState.push_back("line" + ins[i].getArgs()[1]);
															break;
														}
													}
												}
											}

											return result;
											break;
										}
										else
										{
											switch (ins[ifGotoEnd + 2].getCode())
											{
												case opAcc:
												{
													result.numStates = 2;
													for (int c = 0; c < alphabet.size(); ++c)
													{
														if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
														{
															result.write.push_back(alphabet[std::stoi(ins[ifGotoEnd].getArgs()[0])]);
															result.move.push_back(ins[ifGotoEnd + 1].getArgs()[0][0]);
															result.nextState.push_back("a");
														}
														else
														{
															result.write.push_back(alphabet[c]);
															result.write.push_back('r');
															result.nextState.push_back(std::to_string(numStates + 1));
														}
													}

													//TODO: Look-ahead optimization
													for (int c = 0; c < alphabet.size(); ++c)
													{
														result.write.push_back(alphabet[c]);
														result.move.push_back('l');
														if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
														{
															result.nextState.push_back("a");
														}
														else
														{
															for (int i = 0; i < ifGotoEnd; ++i)
															{
																if (std::stoi(ins[i].getArgs()[0]) == c)
																{
																	result.nextState.push_back("line" + ins[i].getArgs()[1]);
																	break;
																}
															}
														}
													}

													return result;
													break;
												}
												case opRej:
												{
													result.numStates = 2;
													for (int c = 0; c < alphabet.size(); ++c)
													{
														if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
														{
															result.write.push_back(alphabet[std::stoi(ins[ifGotoEnd].getArgs()[0])]);
															result.move.push_back(ins[ifGotoEnd + 1].getArgs()[0][0]);
															result.nextState.push_back("r");
														}
														else
														{
															result.write.push_back(alphabet[c]);
															result.write.push_back('r');
															result.nextState.push_back(std::to_string(numStates + 1));
														}
													}

													//TODO: Look-ahead optimization
													for (int c = 0; c < alphabet.size(); ++c)
													{
														result.write.push_back(alphabet[c]);
														result.move.push_back('l');
														if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
														{
															result.nextState.push_back("r");
														}
														else
														{
															for (int i = 0; i < ifGotoEnd; ++i)
															{
																if (std::stoi(ins[i].getArgs()[0]) == c)
																{
																	result.nextState.push_back("line" + ins[i].getArgs()[1]);
																	break;
																}
															}
														}
													}

													return result;
													break;
												}
												case opIfGoto:
												{
													std::vector<char> secondAlphabetSplit;
													for (int i = ifGotoEnd + 2; i < ins.size(); ++i)
													{
														secondAlphabetSplit.push_back(alphabet[std::stoi(ins[i].getArgs()[0])]);
													}

													result.numStates = 2;
													for (int c = 0; c < alphabet.size(); ++c)
													{
														if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
														{
															result.write.push_back(alphabet[std::stoi(ins[ifGotoEnd].getArgs()[0])]);
															result.move.push_back(ins[ifGotoEnd + 1].getArgs()[0][0]);
															if (std::find(secondAlphabetSplit.begin(), secondAlphabetSplit.end(), alphabet[c]) == secondAlphabetSplit.end())
															{
																result.nextState.push_back(std::to_string(numStates + 2));
															}
															else
															{
																for (int i = ifGotoEnd + 2; i < ins.size(); ++i)
																{
																	if (std::stoi(ins[i].getArgs()[0]) == c)
																	{
																		result.nextState.push_back("line" + ins[i].getArgs()[1]);
																		break;
																	}
																}
															}
														}
														else
														{
															result.write.push_back(alphabet[c]);
															result.move.push_back('r');
															result.nextState.push_back(std::to_string(numStates + 1));
														}
													}

													//TODO: Look-ahead optimization
													for (int c = 0; c < alphabet.size(); ++c)
													{
														result.write.push_back(alphabet[c]);
														result.move.push_back('l');
														if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
														{
															if (std::find(secondAlphabetSplit.begin(), secondAlphabetSplit.end(), alphabet[c]) == secondAlphabetSplit.end())
															{
																result.nextState.push_back(std::to_string(numStates + 2));
															}
															else
															{
																for (int i = ifGotoEnd + 2; i < ins.size(); ++i)
																{
																	if (std::stoi(ins[i].getArgs()[0]) == c)
																	{
																		result.nextState.push_back("line" + ins[i].getArgs()[1]);
																		break;
																	}
																}
															}
														}
														else
														{
															for (int i = 0; i < ifGotoEnd; ++i)
															{
																if (std::stoi(ins[i].getArgs()[0]) == c)
																{
																	result.nextState.push_back("line" + ins[i].getArgs()[1]);
																	break;
																}
															}
														}
													}

													return result;
													break;
												}
												case opGoto:
												{
													result.numStates = 2;
													for (int c = 0; c < alphabet.size(); ++c)
													{
														if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
														{
															result.write.push_back(alphabet[std::stoi(ins[ifGotoEnd].getArgs()[0])]);
															result.move.push_back(ins[ifGotoEnd + 1].getArgs()[0][0]);
															result.nextState.push_back("line" + ins[ifGotoEnd + 2].getArgs()[0]);
														}
														else
														{
															result.write.push_back(alphabet[c]);
															result.write.push_back('r');
															result.nextState.push_back(std::to_string(numStates + 1));
														}
													}

													//TODO: Look-ahead optimization
													for (int c = 0; c < alphabet.size(); ++c)
													{
														result.write.push_back(alphabet[c]);
														result.move.push_back('l');
														if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
														{
															result.nextState.push_back("r");
														}
														else
														{
															for (int i = 0; i < ifGotoEnd; ++i)
															{
																if (std::stoi(ins[i].getArgs()[0]) == c)
																{
																	result.nextState.push_back("line" + ins[i].getArgs()[1]);
																	break;
																}
															}
														}
													}

													return result;
													break;
												}
											}
										}

										break;
									}
								}
							}

							break;
						}
						case opMove:
						{
							if (ins.size() == ifGotoEnd + 1)
							{
								result.numStates = 2;
								for (int c = 0; c < alphabet.size(); ++c)
								{
									result.write.push_back(alphabet[c]);
									if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
									{
										result.move.push_back(ins[ifGotoEnd].getArgs()[0][0]);
										result.nextState.push_back(std::to_string(numStates + 2));
									}
									else
									{
										result.move.push_back('r');
										result.nextState.push_back(std::to_string(numStates + 1));
									}
								}

								//TODO: Look-ahead optimization
								for (int c = 0; c < alphabet.size(); ++c)
								{
									result.write.push_back(alphabet[c]);
									result.move.push_back('l');
									if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
									{
										result.nextState.push_back(std::to_string(numStates + 2));
									}
									else
									{
										for (int i = 0; i < ifGotoEnd; ++i)
										{
											if (std::stoi(ins[i].getArgs()[0]) == c)
											{
												result.nextState.push_back("line" + ins[i].getArgs()[1]);
												break;
											}
										}
									}
								}

								return result;
								break;
							}
							else
							{
								switch (ins[ifGotoEnd].getCode())
								{
									case opAcc:
									{
										result.numStates = 2;
										for (int c = 0; c < alphabet.size(); ++c)
										{
											result.write.push_back(alphabet[c]);
											if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
											{
												result.move.push_back(ins[ifGotoEnd].getArgs()[0][0]);
												result.nextState.push_back("a");
											}
											else
											{
												result.move.push_back('r');
												result.nextState.push_back(std::to_string(numStates + 1));
											}
										}
										
										//TODO: Look-ahead optimization
										for (int c = 0; c < alphabet.size(); ++c)
										{
											result.write.push_back(alphabet[c]);
											result.move.push_back('l');
											if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
											{
												result.nextState.push_back("a");
											}
											else
											{
												for (int i = 0; i < ifGotoEnd; ++i)
												{
													if (std::stoi(ins[i].getArgs()[0]) == c)
													{
														result.nextState.push_back("line" + ins[i].getArgs()[1]);
														break;
													}
												}
											}
										}
										
										return result;
										break;
									}
									case opRej:
									{
										result.numStates = 2;
										for (int c = 0; c < alphabet.size(); ++c)
										{
											result.write.push_back(alphabet[c]);
											if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
											{
												result.move.push_back(ins[ifGotoEnd].getArgs()[0][0]);
												result.nextState.push_back("r");
											}
											else
											{
												result.move.push_back('r');
												result.nextState.push_back(std::to_string(numStates + 1));
											}
										}

										//TODO: Look-ahead optimization
										for (int c = 0; c < alphabet.size(); ++c)
										{
											result.write.push_back(alphabet[c]);
											result.move.push_back('l');
											if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
											{
												result.nextState.push_back("r");
											}
											else
											{
												for (int i = 0; i < ifGotoEnd; ++i)
												{
													if (std::stoi(ins[i].getArgs()[0]) == c)
													{
														result.nextState.push_back("line" + ins[i].getArgs()[1]);
														break;
													}
												}
											}
										}

										return result;
										break;
									}
									case opGoto:
									{
										result.numStates = 2;
										for (int c = 0; c < alphabet.size(); ++c)
										{
											result.write.push_back(alphabet[c]);
											if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
											{
												result.move.push_back(ins[ifGotoEnd].getArgs()[0][0]);
												result.nextState.push_back("r");
											}
											else
											{
												result.move.push_back('r');
												result.nextState.push_back(std::to_string(numStates + 1));
											}
										}

										//TODO: Look-ahead optimization
										for (int c = 0; c < alphabet.size(); ++c)
										{
											result.write.push_back(alphabet[c]);
											result.move.push_back('l');
											if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
											{
												result.nextState.push_back("r");
											}
											else
											{
												for (int i = 0; i < ifGotoEnd; ++i)
												{
													if (std::stoi(ins[i].getArgs()[0]) == c)
													{
														result.nextState.push_back("line" + ins[i].getArgs()[1]);
														break;
													}
												}
											}
										}

										return result;
										break;
									}
								}
							}

							break;
						}
						case opGoto:
						{
							result.numStates = 2;
							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[c]);
								result.move.push_back('r');
								if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
								{
									result.nextState.push_back("line" + ins[ifGotoEnd].getArgs()[0]);
								}
								else
								{
									result.nextState.push_back(std::to_string(numStates + 1));
								}
							}

							//TODO: Look-ahead optimization
							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[c]);
								result.move.push_back('l');
								if (std::find(alphabetSplit.begin(), alphabetSplit.end(), alphabet[c]) == alphabetSplit.end())
								{
									result.nextState.push_back("r");
								}
								else
								{
									for (int i = 0; i < ifGotoEnd; ++i)
									{
										if (std::stoi(ins[i].getArgs()[0]) == c)
										{
											result.nextState.push_back("line" + ins[i].getArgs()[1]);
											break;
										}
									}
								}
							}

							return result;
							break;
						}
					}
				}

				break;
			}
			case opWrite:
			{
				if (ins.size() == 1)
				{
					result.numStates = 2;
					
					for (int c = 0; c < alphabet.size(); ++c)
					{
						result.write.push_back(alphabet[std::stoi(ins[0].getArgs()[0])]);
						result.move.push_back('r');
						result.nextState.push_back(std::to_string(numStates + 1));
					}
					for (int c = 0; c < alphabet.size(); ++c)
					{
						result.write.push_back(alphabet[c]);
						result.move.push_back('l');
						result.nextState.push_back(std::to_string(numStates + 2));
					}
				}
				else
				{
					switch (ins[1].getCode())
					{
						case opAcc:
						{
							result.numStates = 2;

							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[std::stoi(ins[0].getArgs()[0])]);
								result.move.push_back('r');
								result.nextState.push_back(std::to_string(numStates + 1));
							}
							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[c]);
								result.move.push_back('l');
								result.nextState.push_back("a");
							}

							return result;
							break;
						}
						case opRej:
						{
							result.numStates = 2;

							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[std::stoi(ins[0].getArgs()[0])]);
								result.move.push_back('r');
								result.nextState.push_back(std::to_string(numStates + 1));
							}
							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[c]);
								result.move.push_back('l');
								result.nextState.push_back("a");
							}
							
							return result;
							break;
						}
						case opMove:
						{
							if (ins.size() == 2)
							{
								result.numStates = 2;

								for (int c = 0; c < alphabet.size(); ++c)
								{
									result.write.push_back(alphabet[std::stoi(ins[0].getArgs()[0])]);
									result.move.push_back('r');
									result.nextState.push_back(std::to_string(numStates + 1));
								}
								for (int c = 0; c < alphabet.size(); ++c)
								{
									result.write.push_back(alphabet[c]);
									result.move.push_back('l');
									result.nextState.push_back("a");
								}

								return result;
								break;
							}
							else
							{
								switch (ins[2].getCode())
								{
									case opAcc:
									{
										result.numStates = 1;
										for (int c = 0; c < alphabet.size(); ++c)
										{
											result.write.push_back(alphabet[std::stoi(ins[0].getArgs()[0])]);
											result.move.push_back(ins[1].getArgs()[0][0]);
											result.nextState.push_back("a");
										}

										return result;
										break;
									}
									case opRej:
									{
										result.numStates = 1;
										for (int c = 0; c < alphabet.size(); ++c)
										{
											result.write.push_back(alphabet[std::stoi(ins[0].getArgs()[0])]);
											result.move.push_back(ins[1].getArgs()[0][0]);
											result.nextState.push_back("r");
										}

										return result;
										break;
									}
									case opGoto:
									{
										result.numStates = 1;

										for (int c = 0; c < alphabet.size(); ++c)
										{
											result.write.push_back(alphabet[std::stoi(ins[0].getArgs()[0])]);
											result.move.push_back(ins[1].getArgs()[0][0]);
											result.nextState.push_back("line" + ins[2].getArgs()[0]);
										}

										return result;
										break;
									}
								}
							}

							break;
						}
						case opGoto:
						{
							result.numStates = 2;

							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[std::stoi(ins[0].getArgs()[0])]);
								result.move.push_back('r');
								result.nextState.push_back(std::to_string(numStates + 1));
							}
							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[c]);
								result.move.push_back('l');
								result.nextState.push_back("line" + ins[1].getArgs()[0]);
							}

							return result;
							break;
						}
					}
				}

				break;
			}
			case opGoto:
			{
				result.numStates = 2;

				for (int c = 0; c < alphabet.size(); ++c)
				{
					result.write.push_back(alphabet[std::stoi(ins[0].getArgs()[0])]);
					result.move.push_back('r');
					result.nextState.push_back(std::to_string(numStates + 1));
				}
				for (int c = 0; c < alphabet.size(); ++c)
				{
					result.write.push_back(alphabet[c]);
					result.move.push_back('l');
					result.nextState.push_back("line" + ins[0].getArgs()[0]);
				}

				return result;
				break;
			}
			case opMove:
			{
				if (ins.size() == 1)
				{
					result.numStates = 1;
					
					for (int c = 0; c < alphabet.size(); ++c)
					{
						result.write.push_back(alphabet[c]);
						result.move.push_back(ins[0].getArgs()[0][0]);
						result.nextState.push_back(std::to_string(numStates + 1));
					}

					return result;
					break;
				}
				else
				{
					switch (ins[1].getCode())
					{
						case opAcc:
						{
							result.numStates = 1;

							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[c]);
								result.move.push_back(ins[0].getArgs()[0][0]);
								result.nextState.push_back("a");
							}

							return result;
							break;
						}
						case opRej:
						{
							result.numStates = 1;

							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[c]);
								result.move.push_back(ins[0].getArgs()[0][0]);
								result.nextState.push_back("r");
							}

							return result;
							break;
						}
						case opGoto:
						{
							result.numStates = 1;

							for (int c = 0; c < alphabet.size(); ++c)
							{
								result.write.push_back(alphabet[c]);
								result.move.push_back(ins[0].getArgs()[0][0]);
								result.nextState.push_back("line" + ins[1].getArgs()[0]);
							}

							return result;
							break;
						}
					}
				}
			}
		}
	}

	return result;
}