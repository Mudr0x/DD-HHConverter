# DD-HHConverter
Hand History converter to 888 Poker format for DD Poker simulator (www.ddpoker.com)


- Insert the hands and summary to convert in original DD HH and summary file respectively named "history.paradise.txt" and "tournaments.csv", copy / paste, or replace, but the files must be called like that.
- The files must be in the same folder as the exe.
- Run the exe, the converted hh and will be written in "history.888.txt" and "tournaments.888.txt", ready to import. 
  (will be overwritten every time)
- Use the ini file "HHConverter.ini" to manually put buy-in division factor (to have the possibilty to import on PT4 with small stacks license), fee %, tourney size (X-Max) and currency. Due to DD HH limitations conversion can only be done for one type of tourney format at once and you can't mix different type of tourney, to use conversion for a different format you must update manually tourney format in ini file and relaunch conversion.
  (each DD HH you want to convert to 888 HH must be for one tourney format and ini file updated with this tourney format)

* I noticed a bug we can't deal with in DD HH if you are on seat 10 and in button, DD HH shows seat 1 is in button instead of seat 10 despite it correctly recognize you are in seat 10.
