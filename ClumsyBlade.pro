TEMPLATE      = subdirs
SUBDIRS       = \
    CTPTrader/CTPTrader.pro \
    DataAccessLayer/DataAccessLayer.pro

# install
sources.files = *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]
INSTALLS += sources

OTHER_FILES += \
    Install/install.nsi
