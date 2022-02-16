// HHConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
#include <regex>

using namespace std;


int main() {

	string lineHH = "", lineSum = "";

	ifstream readHH, readSum, ini;
	ofstream writeHH, writeSum;
	smatch match;
	streampos oldpos;

	//// Manually set those values with your tourney format in Ini file
	double BuyInFactor;
	double FeePct;
	string TourneySize;
	string curr;
	////////////////////////

	double BuyIn, Fee;

	int TourneyNumber = 0;
	int TotalPlayer = 0;
	int Hands = 0;

	string finishPlace = "";
	string totalPlace = "";
	string Profit = "";

	bool bWrite = false;
	bool bNewTourney = false;

	string SmallBlind, BigBlind;
	string PlayerCard1 = "";
	vector< string > ShowedHand[2];
	vector< string > Collected[2];

	string ButtonSeat = "";
	string GameNumber = "";

	//date var
	string Day = "";
	string Month = "";
	string Year = "";

	string Hour = "";
	string Min = "";
	string Sec = "";


	ini.open("HHConverter.ini");
	readHH.open("history.paradise.txt");
	readSum.open("tournaments.csv");
	writeHH.open("history.888.txt");
	writeSum.open("tournaments.888.txt");
	getline(readSum, lineSum);
	getline(readSum, lineSum);

	cout << "Converting..\n\n";

	// check stream status
	if (!readHH) cerr << "Can't open history.paradise.txt file!\n";
	if (!readSum) cerr << "Can't open tournaments.csv!\n";


	while (!ini.eof()) {
		getline(ini, lineHH);
		// get buy-in factor, fee%, tourney size (X-Max) and currency in Ini file
		if (regex_search(lineHH, match, regex("BuyInFactor=([0-9.]+)"))) {
			BuyInFactor = stod(match[1]);
			continue;
		}
		if (regex_search(lineHH, match, regex("FeePct=([0-9.]+)"))) {
			FeePct = stod(match[1]);
			continue;
		}
		if (regex_search(lineHH, match, regex("TourneySize=([0-9]+)"))) {
			TourneySize = match[1];
			continue;
		}
		if (regex_search(lineHH, match, regex("Currency=(.+?)"))) {
			curr = match[1];
			if (curr == "â") curr = "€";
			if (curr == "Â") curr = "€";
			continue;
		}
	}
	ini.close();

	while (!readHH.eof()) {
		getline(readHH, lineHH);
		if (lineHH == "") { continue; }

		// get game number
		if (regex_search(lineHH, match, regex("Game #([0-9]+)"))) {
			GameNumber = match[1];
			// get blinds
			regex_search(lineHH, match, regex("- ([0-9,]+)/([0-9,]+) No Limit"));
			SmallBlind = match[1];
			BigBlind = match[2];
			// get date
			regex_search(lineHH, match, regex("Hold'em - ([0-9]+)/([0-9]+)/([0-9]+)-([0-9]+):([0-9]+):([0-9]+)"));
			Year = match[1];
			Month = match[2];
			Day = match[3];
			Hour = match[4];
			Min = match[5];
			Sec = match[6];

			writeHH << "#Game No : " << GameNumber << endl;
			writeHH << "***** 888poker Hand History for Game " << GameNumber << " *****" << endl;
			writeHH << SmallBlind << "/" << BigBlind << " Blinds No Limit Holdem - *** " << Day << " " << Month << " " << Year << " " << Hour << ":" << Min << ":" << Sec << endl;

			continue;
		}

		// get tourney number		
		if (regex_search(lineHH, match, regex("Hand #([0-9]+)"))) {
			if (match[1] == "1") {
				bNewTourney = true;
				++TourneyNumber;
				if (!readSum.eof()) {
					getline(readSum, lineSum);
					if (regex_search(lineSum, match, regex("^([0-9]+)([a-z ]*)([0-9]+)(.+?):([0-9]+),\\$([0-9]+)(.+?)\\$([0-9\\(\\)]*)$"))) {
						BuyIn = stod(match[6]) / BuyInFactor;
						Fee = BuyIn * (FeePct/100);
						BuyIn = BuyIn - Fee;

						finishPlace = match[1];
						totalPlace = match[3];
						Profit = match[8];
						Profit = regex_replace(Profit, regex("\\("), "-");
						Profit = regex_replace(Profit, regex("\\)"), "");

						writeSum << "***** Tournament Summary *****" << endl;
						writeSum << "Tournament ID: " << TourneyNumber << endl;
						writeSum << "Buy-In: " << curr << BuyIn << " + " << curr << Fee << endl;
					}
				}
			}
			else bNewTourney = false;
			writeHH << "Tournament #" << TourneyNumber << " " << curr << BuyIn << " + " << curr << Fee << " - Table #0 " << TourneySize << " Max (Real Money)" << endl;
			// get button seat
			regex_search(lineHH, match, regex("Seat ([0-9]+)"));
			ButtonSeat = match[1];
			writeHH << "Seat " + ButtonSeat + " is the button" << endl;
			oldpos = readHH.tellg();

			continue;
		}

		// get seats		
		if (regex_search(lineHH, match, regex("^Seat( *)([0-9]+): ([a-zA-Z0-9.-_]+)(.+?)([0-9,]+)"))) {
			++TotalPlayer;
			if (bWrite) {
				TotalPlayer = 0;
				writeHH << "Seat " << match[2] << ": " << match[3] << " ( " << match[5] << " )" << endl;
			}

			continue;
		}
		
		// get total players
		if (TotalPlayer > 0) {
			readHH.seekg(oldpos);
			writeHH << "Total number of players : " << TotalPlayer << endl;
			bWrite = true;

			continue;
		} else bWrite = false;

		// get antes
		if (regex_search(lineHH, match, regex("^([a-zA-Z0-9.-_]+): Ante(.+?)([0-9,]+)"))) {
			writeHH << match[1] << " posts ante [" << match[3] << "]" << endl;

			continue;
		}

		// get small blind
		if (regex_search(lineHH, match, regex("^([a-zA-Z0-9.-_]+): Post Small Blind(.+?)([0-9,]+)"))) {
			writeHH << match[1] << " posts small blind [" << match[3] << "]" << endl;

			continue;
		}

		// get big blind
		if (regex_search(lineHH, match, regex("^([a-zA-Z0-9.-_]+): Post Big Blind(.+?)([0-9,]+)"))) {
			writeHH << match[1] << " posts big blind [" << match[3] << "]" << endl;

			continue;
		}

		// get preflop
		if (lineHH == "Dealing...") {
			writeHH << "** Dealing down cards **" << endl;

			continue;
		}
		if (regex_search(lineHH, match, regex("^Dealt to ([a-zA-Z0-9.-_]+)(.+?)([a-zA-Z0-9]+)"))) {
			if (PlayerCard1 != "") {
				writeHH  << "Dealt to " << match[1] << " [ " + PlayerCard1 + ", " << match[3] << " ]" << endl;
				PlayerCard1 = "";
				double profit = stod(Profit) / BuyInFactor;
				if (bNewTourney) {
					if (profit < 0) {
						Profit = regex_replace(Profit, regex("-"), "");
						writeSum << match[1] << " finished " << finishPlace << "/" << totalPlace << " and lost " << curr << fabs(profit) << endl << endl;
					} else 

						writeSum << match[1] << " finished " << finishPlace << "/" << totalPlace << " and won " << curr << profit << endl << endl;
				}

				continue;
			}
			PlayerCard1 = match[3];

			continue;
		}
		if (regex_search(lineHH, match, regex("^([a-zA-Z0-9.-_]+): Raise(.+?)([0-9,]+)"))) {
			writeHH << match[1] << " raises [" << match[3] << "]" << endl;

			continue;
		}
		if (regex_search(lineHH, match, regex("^([a-zA-Z0-9.-_]+): Bet(.+?)([0-9,]+)"))) {
			writeHH << match[1] << " bets [" << match[3] << "]" << endl;

			continue;
		}
		if (regex_search(lineHH, match, regex("^([a-zA-Z0-9.-_]+): Call(.+?)([0-9,]+)"))) {
			writeHH << match[1] << " calls [" << match[3] << "]" << endl;

			continue;
		}
		if (regex_search(lineHH, match, regex("^([a-zA-Z0-9.-_]+): Check"))) {
			writeHH << match[1] << " checks" << endl;

			continue;
		}
		if (regex_search(lineHH, match, regex("^([a-zA-Z0-9.-_]+): Fold"))) {
			writeHH << match[1] << " folds" << endl;

			continue;
		}

		// get flop
		if (regex_search(lineHH, match, regex("^\\*\\*\\* FLOP \\*\\*\\* : \\[ ([a-zA-Z0-9 ]+) \\]"))) {
			writeHH << "** Dealing flop ** [ " << regex_replace(string(match[1]), regex(" "), ", ") << " ]" << endl;

			continue;
		}

		// get turn
		if (regex_search(lineHH, match, regex("^\\*\\*\\* TURN \\*\\*\\* : \\[([a-zA-Z0-9 ]+)\\] \\[([a-zA-Z0-9 ]+)"))) {
			writeHH << "** Dealing turn ** [" << match[2] << "]" << endl;

			continue;
		}

		// get river
		if (regex_search(lineHH, match, regex("^\\*\\*\\* RIVER \\*\\*\\* : \\[([a-zA-Z0-9 ]+)\\] \\[([a-zA-Z0-9 ]+)"))) {
			writeHH << "** Dealing river ** [" << match[2] << "]" << endl;

			continue;
		}

		// get summary
		if (lineHH == "*** SUMMARY ***") {
			writeHH << "** Summary **" << endl;

			continue;
		}
		lineHH = regex_replace(lineHH, regex("Pot: ([0-9,]+)"), "");
		if (regex_search(lineHH, match, regex("([a-zA-Z0-9.-_]+)(.+?)collected ([0-9,]+),(.+?)\\(showed hand\\) \\[ ([a-zA-Z0-9 ]+) \\]"))) {
			Collected[0].push_back(match[1]);
			Collected[1].push_back(match[3]);
			ShowedHand[0].push_back(match[1]);
			ShowedHand[1].push_back(match[5]);

			continue;
		}
		if (regex_search(lineHH, match, regex("([a-zA-Z0-9.-_]+)(.+?)collected ([0-9,]+),"))) {
			Collected[0].push_back(match[1]);
			Collected[1].push_back(match[3]);

			continue;
		}
		if (regex_search(lineHH, match, regex("([a-zA-Z0-9.-_]+)(.+?)\\(showed hand\\) \\[ ([a-zA-Z0-9 ]+) \\]"))) {
			ShowedHand[0].push_back(match[1]);
			ShowedHand[1].push_back(match[3]);

			continue;
		}

		// writeHH summary
		if (lineHH == "-----------------------------------------------------") {
			for (int i = 0; i < ShowedHand[0].size(); i++) {
				writeHH << ShowedHand[0][i] << " shows [ " << regex_replace(ShowedHand[1][i], regex(" "), ", ") << " ]" << endl;
			}
			for (int i = 0; i < Collected[0].size(); i++) {
				writeHH << Collected[0][i] << " collected [ " << Collected[1][i] << " ]" << endl;
			}
			writeHH << endl << endl << endl;
			memset(ShowedHand, 0x00, sizeof(ShowedHand[0]) * 2);
			memset(Collected, 0x00, sizeof(Collected[0]) * 2);

			++Hands;
			continue;
		}
	}

	cout << "Finish! \nHands Converted: " << Hands << endl;
	readHH.close();
	writeHH.close();
	readSum.close();
	writeSum.close();
	cout << "Press Enter to Exit.\n";
	cin.ignore();
}

