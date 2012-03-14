/** @file

MODULE				: RunBeforeH264VlcDecoder

TAG						: RBH264VD

FILE NAME			: RunBeforeH264VlcDecoder.cpp

DESCRIPTION		: A run before Vlc decoder implementation as defined in
								H.264 Recommendation (03/2005) Table 9.10 page 206. The 
								zero coeffs left defines which vlc array to use. This 
								implementation is implemented with an IVlcEncoder 
								Interface.

LICENSE	: GNU Lesser General Public License

Copyright (c) 2008 - 2012, CSIR
All rights reserved.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

===========================================================================
*/
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#else
#include <stdio.h>
#endif

#include "RunBeforeH264VlcDecoder.h"

/*
---------------------------------------------------------------------------
	Construction and destruction.
---------------------------------------------------------------------------
*/
RunBeforeH264VlcDecoder::RunBeforeH264VlcDecoder(void)
{
	_numCodeBits	= 0;
}//end constructor.

RunBeforeH264VlcDecoder::~RunBeforeH264VlcDecoder(void)
{
}//end destructor.

/*
---------------------------------------------------------------------------
	Interface Methods.
---------------------------------------------------------------------------
*/
/** Decode run before - zeros left symbols from the bit stream.
The first 2 param symbols represent run before and zeros left, respectively. The 
zeros left is an input that selects the appropriate vlc array for the run before
decoding. For fast implementation the tree splitting is done in the code.
@param bsr			: Bit stream to get from.
@param symbol1	: Returned run before for this coeff.
@param symbol2	: Zeros left for this block.
@return					: Num of bits extracted.
*/
int	RunBeforeH264VlcDecoder::Decode2(IBitStreamReader* bsr, int* symbol1, int* symbol2)
{
	int zerosLeft	= *symbol2;	///< Used as an input.

	/// Reset bits extracted.
  _numCodeBits	= 0;
	int rB				= 0;

	/// Select the method depending on the total number of coeffs.
	switch(zerosLeft)
	{
		case 1:
			{
				if(bsr->Read())	///< 1
					rB = 0;
				else						///< 0
					rB = 1;
				_numCodeBits = 1;
			}//end case 1...
			break;
		case 2:
			{
				if(bsr->Read())	///< 1
				{
					_numCodeBits = 1; rB = 0;
				}//end if 1...
				else						///< 0
				{
					if(bsr->Read())	///< 01
					{
						_numCodeBits = 2; rB = 1;
					}//end if 01...
					else						///< 00
					{
						_numCodeBits = 2; rB = 2;
					}//end else 00...
				}//end else 0...
			}//end case 2...
			break;
		case 3:
			{
				if(bsr->Read())	///< 1
				{
					if(bsr->Read())	///< 11
						rB = 0;
					else						///< 10
						rB = 1;
				}//end if 1...
				else						///< 0
				{
					if(bsr->Read())	///< 01
						rB = 2;
					else						///< 00
						rB = 3;
				}//end else 0...
				_numCodeBits = 2;
			}//end case 3...
			break;
		case 4:
			{
				if(bsr->Read())	///< 1
				{
					if(bsr->Read())	///< 11
					{
						_numCodeBits = 2; rB = 0;
					}//end if 11...
					else						///< 10
					{
						_numCodeBits = 2; rB = 1;
					}//end else 10...
				}//end if 1...
				else						///< 0
				{
					if(bsr->Read())	///< 01
					{
						_numCodeBits = 2; rB = 2;
					}//end if 01...
					else						///< 00
					{
						if(bsr->Read())	///< 001
						{
							_numCodeBits = 3; rB = 3;
						}//end if 001...
						else						///< 000
						{
							_numCodeBits = 3; rB = 4;
						}//end else 000...
					}//end else 00...
				}//end else 0...
			}//end case 4...
			break;
		case 5:
			{
				if(bsr->Read())	///< 1
				{
					if(bsr->Read())	///< 11
					{
						_numCodeBits = 2; rB = 0;
					}//end if 11...
					else						///< 10
					{
						_numCodeBits = 2; rB = 1;
					}//end else 10...
				}//end if 1...
				else						///< 0
				{
					if(bsr->Read())	///< 01
					{
						if(bsr->Read())	///< 011
						{
							_numCodeBits = 3; rB = 2;
						}//end if 011...
						else						///< 010
						{
							_numCodeBits = 3; rB = 3;
						}//end else 010...
					}//end if 01...
					else						///< 00
					{
						if(bsr->Read())	///< 001
						{
							_numCodeBits = 3; rB = 4;
						}//end if 001...
						else						///< 000
						{
							_numCodeBits = 3; rB = 5;
						}//end else 000...
					}//end else 00...
				}//end else 0...
			}//end case 5...
			break;
		case 6:
			{
				if(bsr->Read())	///< 1
				{
					if(bsr->Read())	///< 11
					{
						_numCodeBits = 2; rB = 0;
					}//end if 11...
					else						///< 10
					{
						if(bsr->Read())	///< 101
						{
							_numCodeBits = 3; rB = 5;
						}//end if 101...
						else						///< 100
						{
							_numCodeBits = 3; rB = 6;
						}//end else 100...
					}//end else 10...
				}//end if 1...
				else						///< 0
				{
					if(bsr->Read())	///< 01
					{
						if(bsr->Read())	///< 011
						{
							_numCodeBits = 3; rB = 3;
						}//end if 011...
						else						///< 010
						{
							_numCodeBits = 3; rB = 4;
						}//end else 010...
					}//end if 01...
					else						///< 00
					{
						if(bsr->Read())	///< 001
						{
							_numCodeBits = 3; rB = 2;
						}//end if 001...
						else						///< 000
						{
							_numCodeBits = 3; rB = 1;
						}//end else 000...
					}//end else 00...
				}//end else 0...
			}//end case 6...
			break;
		default:	///< Greater than 6.
			{
				if(zerosLeft > 6)
				{
					if(bsr->Read())	///< 1
					{
						if(bsr->Read())	///< 11
						{
							if(bsr->Read())	///< 111
							{
								_numCodeBits = 3; rB =0 ;
							}//end if 111...
							else						///< 110
							{
								_numCodeBits = 3; rB = 1;
							}//end else 110...
						}//end if 11...
						else						///< 10
						{
							if(bsr->Read())	///< 101
							{
								_numCodeBits = 3; rB = 2;
							}//end if 101...
							else						///< 100
							{
								_numCodeBits = 3; rB = 3;
							}//end else 100...
						}//end else 10...
					}//end if 1...
					else						///< 0
					{
						if(bsr->Read())	///< 01
						{
							if(bsr->Read())	///< 011
							{
								_numCodeBits = 3; rB = 4;
							}//end if 011...
							else						///< 010
							{
								_numCodeBits = 3; rB = 5;
							}//end else 010...
						}//end if 01...
						else						///< 00
						{
							if(bsr->Read())	///< 001
							{
								_numCodeBits = 3; rB = 6;
							}//end if 001...
							else						///< 000
							{
								if(bsr->Read())	///< 0001
								{
									_numCodeBits = 4; rB = 7;
								}//end if 0001...
								else						///< 0000
								{
									if(bsr->Read())	///< 0000 1
									{
										_numCodeBits = 5; rB = 8;
									}//end if 0000 1...
									else						///< 0000 0
									{
										if(bsr->Read())	///< 0000 01
										{
											_numCodeBits = 6; rB = 9;
										}//end if 0000 01...
										else						///< 0000 00
										{
											if(bsr->Read())	///< 0000 001
											{
												_numCodeBits = 7; rB = 10;
											}//end if 0000 001...
											else						///< 0000 000
											{
												if(bsr->Read())	///< 0000 0001
												{
													_numCodeBits = 8; rB = 11;
												}//end if 0000 0001...
												else						///< 0000 0000
												{
													if(bsr->Read())	///< 0000 0000 1
													{
														_numCodeBits = 9; rB = 12;
													}//end if 0000 0000 1...
													else						///< 0000 0000 0
													{
														if(bsr->Read())	///< 0000 0000 01
														{
															_numCodeBits = 10; rB = 13;
														}//end if 0000 0000 01...
														else						///< 0000 0000 00
														{
															if(bsr->Read())	///< 0000 0000 001
															{
																_numCodeBits = 11; rB = 14;
															}//end if 0000 0000 001...
														}//end else 0000 0000 00...
													}//end else 0000 0000 0...
												}//end else 0000 0000...
											}//end else 0000 000...
										}//end else 0000 00...
									}//end else 0000 0...
								}//end else 0000...
							}//end else 000...
						}//end else 00...
					}//end else 0...
				}//end if zerosLeft...
			}//end case >6...
			break;
	}//end switch zerosLeft...

	/// Load 'em up.
	*symbol1 = rB;

  return(_numCodeBits);
}//end Decode2.


