cd CTPTrader
del *.vc*proj
qmake -spec win32-msvc2008 -tp vc

cd ../DataAccessLayer
del *.vc*proj
qmake -spec win32-msvc2008 -tp vc

cd ..
del *.sln
qmake -spec win32-msvc2008 -tp vc

pause