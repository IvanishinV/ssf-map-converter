
#include "stdafx.h"

#include "Beach.h"
#include "util.h"

void beachrhombs(const std::string_view& map_rhombs, std::ofstream& outputFile)
{
	const size_t length = map_rhombs.size();
	std::vector<uint8_t> v(length * 2, 0);

	for (size_t offset = 0, vOffset = 0; offset < length; offset += 2, vOffset += 4)
	{
		const uint8_t num1 = map_rhombs[offset];
		const uint8_t num2 = map_rhombs[offset + 1];

		// Tile_col это байт €ркости
		const uint8_t Tile_col = num2 / 8;

		uint8_t Tile_num{}, Tile_type{};
		if (num2 % 2 == 0)
		{
			if (num1 >= 0 && num1 <= 255)
			{
				//GRASS1
				//---------------------------------------------------------------------------------------------------------
				if (num1 >= 0 && num1 <= 17)
				{
					Tile_num = 0x00;
					Tile_type = 0x00;
					// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}

				//GRASS2
				//---------------------------------------------------------------------------------------------------------
				else
					if (num1 >= 30 && num1 <= 38)
					{
						Tile_num = 0x14;
						Tile_type = 0x00;
						// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
					}
					else
						if (num1 == 19)
						{
							Tile_num = 0x14;
							Tile_type = 0x01;
							// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
						}
						else
							if (num1 == 26)
							{
								Tile_num = 0x14;
								Tile_type = 0x02;
								// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
							}
							else
								if (num1 == 21)
								{
									Tile_num = 0x14;
									Tile_type = 0x03;
									// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
								}
								else
									if (num1 == 27)
									{
										Tile_num = 0x14;
										Tile_type = 0x04;
										// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
									}
									else
										if (num1 == 20)
										{
											Tile_num = 0x14;
											Tile_type = 0x05;
											// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
										}
										else
											if (num1 == 29)
											{
												Tile_num = 0x14;
												Tile_type = 0x06;
												// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
											}
											else
												if (num1 == 18)
												{
													Tile_num = 0x14;
													Tile_type = 0x07;
													// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
												}
												else
													if (num1 == 28)
													{
														Tile_num = 0x14;
														Tile_type = 0x08;
														// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
													}
													else
														if (num1 == 24)
														{
															Tile_num = 0x14;
															Tile_type = 0x09;
															// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
														}
														else
															if (num1 == 22)
															{
																Tile_num = 0x14;
																Tile_type = 0x0A;
																// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
															}
															else
																if (num1 == 23)
																{
																	Tile_num = 0x14;
																	Tile_type = 0x0B;
																	// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																}
																else
																	if (num1 == 25)
																	{
																		Tile_num = 0x14;
																		Tile_type = 0x0C;
																		// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																	}

				//FIELD
				//---------------------------------------------------------------------------------------------------------
																	else
																		if (num1 == 39 || num1 == 40 || num1 == 41 || num1 >= 54 && num1 <= 60)
																		{
																			Tile_num = 0x28;
																			Tile_type = 0x00;
																			// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																		}
																		else
																			if (num1 == 43 || num1 == 62 || num1 == 74 || num1 == 86)
																			{
																				Tile_num = 0x28;
																				Tile_type = 0x01;
																				// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																			}
																			else
																				if (num1 == 50 || num1 == 69 || num1 == 81 || num1 == 93)
																				{
																					Tile_num = 0x28;
																					Tile_type = 0x02;
																					// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																				}
																				else
																					if (num1 == 42 || num1 == 61 || num1 == 73 || num1 == 85)
																					{
																						Tile_num = 0x28;
																						Tile_type = 0x03;
																						// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																					}
																					else
																						if (num1 == 52 || num1 == 71 || num1 == 83 || num1 == 95)
																						{
																							Tile_num = 0x28;
																							Tile_type = 0x04;
																							// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																						}
																						else
																							if (num1 == 44 || num1 == 63 || num1 == 75 || num1 == 87)
																							{
																								Tile_num = 0x28;
																								Tile_type = 0x05;
																								// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																							}
																							else
																								if (num1 == 51 || num1 == 70 || num1 == 82 || num1 == 94)
																								{
																									Tile_num = 0x28;
																									Tile_type = 0x06;
																									// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																								}
																								else
																									if (num1 == 45 || num1 == 64 || num1 == 76 || num1 == 88)
																									{
																										Tile_num = 0x28;
																										Tile_type = 0x07;
																										// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																									}
																									else
																										if (num1 == 53 || num1 == 72 || num1 == 84 || num1 == 96)
																										{
																											Tile_num = 0x28;
																											Tile_type = 0x08;
																											// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																										}
																										else
																											if (num1 == 49 || num1 == 68 || num1 == 80 || num1 == 92)
																											{
																												Tile_num = 0x28;
																												Tile_type = 0x09;
																												// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																											}
																											else
																												if (num1 == 46 || num1 == 65 || num1 == 77 || num1 == 89)
																												{
																													Tile_num = 0x28;
																													Tile_type = 0x0A;
																													// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																												}
																												else
																													if (num1 == 48 || num1 == 67 || num1 == 79 || num1 == 91)
																													{
																														Tile_num = 0x28;
																														Tile_type = 0x0B;
																														// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																													}
																													else
																														if (num1 == 47 || num1 == 66 || num1 == 78 || num1 == 90)
																														{
																															Tile_num = 0x28;
																															Tile_type = 0x0C;
																															// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																														}
				//SWAMP
				//---------------------------------------------------------------------------------------------------------
																														else
																															if (num1 >= 157 && num1 <= 176)
																															{
																																Tile_num = 0x37;
																																Tile_type = 0x00;
																																// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																															}
																															else
																																if (num1 == 98 || num1 == 110 || num1 == 122 || num1 == 134 || num1 == 146)
																																{
																																	Tile_num = 0x37;
																																	Tile_type = 0x01;
																																	// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																}
																																else
																																	if (num1 == 105 || num1 == 117 || num1 == 129 || num1 == 141 || num1 == 153)
																																	{
																																		Tile_num = 0x37;
																																		Tile_type = 0x02;
																																		// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																	}
																																	else
																																		if (num1 == 97 || num1 == 109 || num1 == 121 || num1 == 133 || num1 == 145)
																																		{
																																			Tile_num = 0x37;
																																			Tile_type = 0x03;
																																			// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																		}
																																		else
																																			if (num1 == 107 || num1 == 119 || num1 == 131 || num1 == 143 || num1 == 155)
																																			{
																																				Tile_num = 0x37;
																																				Tile_type = 0x04;
																																				// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																			}
																																			else
																																				if (num1 == 99 || num1 == 111 || num1 == 123 || num1 == 135 || num1 == 147)
																																				{
																																					Tile_num = 0x37;
																																					Tile_type = 0x05;
																																					// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																				}
																																				else
																																					if (num1 == 106 || num1 == 118 || num1 == 130 || num1 == 142 || num1 == 154)
																																					{
																																						Tile_num = 0x37;
																																						Tile_type = 0x06;
																																						// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																					}
																																					else
																																						if (num1 == 100 || num1 == 112 || num1 == 124 || num1 == 136 || num1 == 148)
																																						{
																																							Tile_num = 0x37;
																																							Tile_type = 0x07;
																																							// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																						}
																																						else
																																							if (num1 == 108 || num1 == 120 || num1 == 132 || num1 == 144 || num1 == 156)
																																							{
																																								Tile_num = 0x37;
																																								Tile_type = 0x08;
																																								// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																							}
																																							else
																																								if (num1 == 104 || num1 == 116 || num1 == 128 || num1 == 140 || num1 == 152)
																																								{
																																									Tile_num = 0x37;
																																									Tile_type = 0x09;
																																									// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																								}
																																								else
																																									if (num1 == 101 || num1 == 113 || num1 == 125 || num1 == 137 || num1 == 149)
																																									{
																																										Tile_num = 0x37;
																																										Tile_type = 0x0A;
																																										// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																									}
																																									else
																																										if (num1 == 103 || num1 == 115 || num1 == 127 || num1 == 139 || num1 == 151)
																																										{
																																											Tile_num = 0x37;
																																											Tile_type = 0x0B;
																																											// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																										}
																																										else
																																											if (num1 == 102 || num1 == 114 || num1 == 126 || num1 == 138 || num1 == 150)
																																											{
																																												Tile_num = 0x37;
																																												Tile_type = 0x0C;
																																												// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																											}
				//GROUND1
				//---------------------------------------------------------------------------------------------------------
																																											else
																																												if (num1 >= 189 && num1 <= 200)
																																												{
																																													Tile_num = 0x46;
																																													Tile_type = 0x00;
																																													// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																												}
																																												else
																																													if (num1 == 179)
																																													{
																																														Tile_num = 0x46;
																																														Tile_type = 0x01;
																																														// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																													}
																																													else
																																														if (num1 == 181)
																																														{
																																															Tile_num = 0x46;
																																															Tile_type = 0x02;
																																															// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																														}
																																														else
																																															if (num1 == 177)
																																															{
																																																Tile_num = 0x46;
																																																Tile_type = 0x03;
																																																// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																															}
																																															else
																																																if (num1 == 182)
																																																{
																																																	Tile_num = 0x46;
																																																	Tile_type = 0x04;
																																																	// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																}
																																																else
																																																	if (num1 == 178)
																																																	{
																																																		Tile_num = 0x46;
																																																		Tile_type = 0x05;
																																																		// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																	}
																																																	else
																																																		if (num1 == 184)
																																																		{
																																																			Tile_num = 0x46;
																																																			Tile_type = 0x06;
																																																			// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																		}
																																																		else
																																																			if (num1 == 180)
																																																			{
																																																				Tile_num = 0x46;
																																																				Tile_type = 0x07;
																																																				// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																			}
																																																			else
																																																				if (num1 == 183)
																																																				{
																																																					Tile_num = 0x46;
																																																					Tile_type = 0x08;
																																																					// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																				}
																																																				else
																																																					if (num1 == 187)
																																																					{
																																																						Tile_num = 0x46;
																																																						Tile_type = 0x09;
																																																						// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																					}
																																																					else
																																																						if (num1 == 185)
																																																						{
																																																							Tile_num = 0x46;
																																																							Tile_type = 0x0A;
																																																							// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																						}
																																																						else
																																																							if (num1 == 186)
																																																							{
																																																								Tile_num = 0x46;
																																																								Tile_type = 0x0B;
																																																								// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																							}
																																																							else
																																																								if (num1 == 188)
																																																								{
																																																									Tile_num = 0x46;
																																																									Tile_type = 0x0C;
																																																									// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																								}
				//GROUND2
				//---------------------------------------------------------------------------------------------------------
																																																								else
																																																									if (num1 >= 201 && num1 <= 209)
																																																									{
																																																										Tile_num = 0x55;
																																																										Tile_type = 0x00;
																																																										// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																									}
																																																									else
																																																										if (num1 == 211 || num1 == 223 || num1 == 235 || num1 == 247)
																																																										{
																																																											Tile_num = 0x55;
																																																											Tile_type = 0x01;
																																																											// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																										}
																																																										else
																																																											if (num1 == 218 || num1 == 230 || num1 == 242 || num1 == 254)
																																																											{
																																																												Tile_num = 0x55;
																																																												Tile_type = 0x02;
																																																												// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																											}
																																																											else
																																																												if (num1 == 210 || num1 == 222 || num1 == 234 || num1 == 246)
																																																												{
																																																													Tile_num = 0x55;
																																																													Tile_type = 0x03;
																																																													// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																												}
																																																												else
																																																													if (num1 == 220 || num1 == 232 || num1 == 244)
																																																													{
																																																														Tile_num = 0x55;
																																																														Tile_type = 0x04;
																																																														// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																													}
																																																													else
																																																														if (num1 == 212 || num1 == 224 || num1 == 236 || num1 == 248)
																																																														{
																																																															Tile_num = 0x55;
																																																															Tile_type = 0x05;
																																																															// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																														}
																																																														else
																																																															if (num1 == 219 || num1 == 231 || num1 == 243 || num1 == 255)
																																																															{
																																																																Tile_num = 0x55;
																																																																Tile_type = 0x06;
																																																																// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																															}
																																																															else
																																																																if (num1 == 213 || num1 == 225 || num1 == 237 || num1 == 249)
																																																																{
																																																																	Tile_num = 0x55;
																																																																	Tile_type = 0x07;
																																																																	// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																																}
																																																																else
																																																																	if (num1 == 221 || num1 == 233 || num1 == 245)
																																																																	{
																																																																		Tile_num = 0x55;
																																																																		Tile_type = 0x08;
																																																																		// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																																	}
																																																																	else
																																																																		if (num1 == 217 || num1 == 229 || num1 == 241 || num1 == 253)
																																																																		{
																																																																			Tile_num = 0x55;
																																																																			Tile_type = 0x09;
																																																																			// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																																		}
																																																																		else
																																																																			if (num1 == 214 || num1 == 226 || num1 == 238 || num1 == 250)
																																																																			{
																																																																				Tile_num = 0x55;
																																																																				Tile_type = 0x0A;
																																																																				// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																																			}
																																																																			else
																																																																				if (num1 == 216 || num1 == 228 || num1 == 240 || num1 == 252)
																																																																				{
																																																																					Tile_num = 0x55;
																																																																					Tile_type = 0x0B;
																																																																					// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																																				}
																																																																				else
																																																																					if (num1 == 215 || num1 == 227 || num1 == 239 || num1 == 251)
																																																																					{
																																																																						Tile_num = 0x55;
																																																																						Tile_type = 0x0C;
																																																																						// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																																					}
																																																																					else
																																																																					{
																																																																						Tile_num = 0x00;
																																																																						Tile_type = 0x00;
																																																																						// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																																																					}
			}
		}
		else
		{
			if (num1 >= 0 && num1 <= 142)
			{
				//GROUND2
				//---------------------------------------------------------------------------------------------------------	
				if (num1 == 3)
				{
					Tile_num = 0x55;
					Tile_type = 0x01;
					// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
					if (num1 == 10)
					{
						Tile_num = 0x55;
						Tile_type = 0x02;
						// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
					}
					else
						if (num1 == 2)
						{
							Tile_num = 0x55;
							Tile_type = 0x03;
							// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
						}
						else
							if (num1 == 0 || num1 == 12)
							{
								Tile_num = 0x55;
								Tile_type = 0x04;
								// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
							}
							else
								if (num1 == 4)
								{
									Tile_num = 0x55;
									Tile_type = 0x05;
									// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
								}
								else
									if (num1 == 11)
									{
										Tile_num = 0x55;
										Tile_type = 0x06;
										// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
									}
									else
										if (num1 == 5)
										{
											Tile_num = 0x55;
											Tile_type = 0x07;
											// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
										}
										else
											if (num1 == 1 || num1 == 13)
											{
												Tile_num = 0x55;
												Tile_type = 0x08;
												// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
											}
											else
												if (num1 == 9)
												{
													Tile_num = 0x55;
													Tile_type = 0x09;
													// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
												}
												else
													if (num1 == 6)
													{
														Tile_num = 0x55;
														Tile_type = 0x0A;
														// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
													}
													else
														if (num1 == 8)
														{
															Tile_num = 0x55;
															Tile_type = 0x0B;
															// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
														}
														else
															if (num1 == 7)
															{
																Tile_num = 0x55;
																Tile_type = 0x0C;
																// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
															}

				//WATER
				//---------------------------------------------------------------------------------------------------------
															else
																if (num1 >= 14 && num1 <= 21)
																{
																	Tile_num = 0x66;
																	Tile_type = 0x00;
																	// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																}
																else
																	if (num1 == 23 || num1 == 35 || num1 == 47 || num1 == 59 || num1 == 71)
																	{
																		Tile_num = 0x66;
																		Tile_type = 0x01;
																		// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																	}
																	else
																		if (num1 == 30 || num1 == 42 || num1 == 54 || num1 == 66 || num1 == 78)
																		{
																			Tile_num = 0x66;
																			Tile_type = 0x02;
																			// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																		}
																		else
																			if (num1 == 22 || num1 == 34 || num1 == 46 || num1 == 58 || num1 == 70)
																			{
																				Tile_num = 0x66;
																				Tile_type = 0x03;
																				// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																			}
																			else
																				if (num1 == 32 || num1 == 44 || num1 == 56 || num1 == 68 || num1 == 80)
																				{
																					Tile_num = 0x66;
																					Tile_type = 0x04;
																					// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																				}
																				else
																					if (num1 == 24 || num1 == 36 || num1 == 48 || num1 == 60 || num1 == 72)
																					{
																						Tile_num = 0x66;
																						Tile_type = 0x05;
																						// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																					}
																					else
																						if (num1 == 31 || num1 == 43 || num1 == 55 || num1 == 67 || num1 == 79)
																						{
																							Tile_num = 0x66;
																							Tile_type = 0x06;
																							// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																						}
																						else
																							if (num1 == 25 || num1 == 37 || num1 == 49 || num1 == 61 || num1 == 73)
																							{
																								Tile_num = 0x66;
																								Tile_type = 0x07;
																								// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																							}
																							else
																								if (num1 == 33 || num1 == 45 || num1 == 57 || num1 == 69 || num1 == 81)
																								{
																									Tile_num = 0x66;
																									Tile_type = 0x08;
																									// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																								}
																								else
																									if (num1 == 29 || num1 == 41 || num1 == 53 || num1 == 65 || num1 == 77)
																									{
																										Tile_num = 0x66;
																										Tile_type = 0x09;
																										// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																									}
																									else
																										if (num1 == 26 || num1 == 38 || num1 == 50 || num1 == 62 || num1 == 74)
																										{
																											Tile_num = 0x66;
																											Tile_type = 0x0A;
																											// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																										}
																										else
																											if (num1 == 28 || num1 == 40 || num1 == 52 || num1 == 64 || num1 == 76)
																											{
																												Tile_num = 0x66;
																												Tile_type = 0x0B;
																												// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																											}
																											else
																												if (num1 == 27 || num1 == 39 || num1 == 51 || num1 == 63 || num1 == 75)
																												{
																													Tile_num = 0x66;
																													Tile_type = 0x0C;
																													// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																												}
				//SAND
				//---------------------------------------------------------------------------------------------------------
																												else
																													if (num1 >= 82 && num1 <= 93 || num1 == 142)
																													{
																														Tile_num = 0x78;
																														Tile_type = 0x00;
																														// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																													}
																													else
																														if (num1 == 95 || num1 == 107 || num1 == 119 || num1 == 131)
																														{
																															Tile_num = 0x78;
																															Tile_type = 0x01;
																															// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																														}
																														else
																															if (num1 == 102 || num1 == 114 || num1 == 126 || num1 == 138)
																															{
																																Tile_num = 0x78;
																																Tile_type = 0x02;
																																// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																															}
																															else
																																if (num1 == 94 || num1 == 106 || num1 == 118 || num1 == 130)
																																{
																																	Tile_num = 0x78;
																																	Tile_type = 0x03;
																																	// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																}
																																else
																																	if (num1 == 104 || num1 == 116 || num1 == 128 || num1 == 140)
																																	{
																																		Tile_num = 0x78;
																																		Tile_type = 0x04;
																																		// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																	}
																																	else
																																		if (num1 == 96 || num1 == 108 || num1 == 120 || num1 == 132)
																																		{
																																			Tile_num = 0x78;
																																			Tile_type = 0x05;
																																			// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																		}
																																		else
																																			if (num1 == 103 || num1 == 115 || num1 == 127 || num1 == 139)
																																			{
																																				Tile_num = 0x78;
																																				Tile_type = 0x06;
																																				// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																			}
																																			else
																																				if (num1 == 97 || num1 == 109 || num1 == 121 || num1 == 133)
																																				{
																																					Tile_num = 0x78;
																																					Tile_type = 0x07;
																																					// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																				}
																																				else
																																					if (num1 == 105 || num1 == 117 || num1 == 129 || num1 == 141)
																																					{
																																						Tile_num = 0x78;
																																						Tile_type = 0x08;
																																						// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																					}
																																					else
																																						if (num1 == 101 || num1 == 113 || num1 == 125 || num1 == 137)
																																						{
																																							Tile_num = 0x78;
																																							Tile_type = 0x09;
																																							// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																						}
																																						else
																																							if (num1 == 98 || num1 == 110 || num1 == 122 || num1 == 134)
																																							{
																																								Tile_num = 0x78;
																																								Tile_type = 0x0A;
																																								// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																							}
																																							else
																																								if (num1 == 100 || num1 == 112 || num1 == 124 || num1 == 136)
																																								{
																																									Tile_num = 0x78;
																																									Tile_type = 0x0B;
																																									// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																								}
																																								else
																																									if (num1 == 99 || num1 == 111 || num1 == 123 || num1 == 135)
																																									{
																																										Tile_num = 0x78;
																																										Tile_type = 0x0C;
																																										// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																									}
																																									else
																																									{
																																										Tile_num = 0x00;
																																										Tile_type = 0x00;
																																										// outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
																																									}
			}
		}
		
		v[vOffset] = Tile_num;
		v[vOffset + 1] = Tile_type;
		v[vOffset + 2] = Tile_col;
		// v[vOffset + 3] = 0;	// not needed since all elements are 0's already
	}

	outputFile.write((char*)v.data(), v.size());
}