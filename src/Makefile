LIBOBJ= QMessageClient.o QMessage.o  QMessageClientList.o miMessage.o qtClientButton.o qtClientDialog.o miTimeSpinBox.o qbLoginInfo.o qbLoginDialog.o miLogFile.o miSliderWidget.o

MOCOBJ=$(LIBOBJ:.o=_moc.o)
LIBSRC=$(LIBOBJ:.o=.cc)


.SUFFIXES: .cc .o

.cc.o:
	$(CXX) $(CCFLAGS) -c $< $(INCLUDE)

all: mocs $(LIBDIR)/libqUtilities.a 

depends:
	rm -f $(DEPENDSFILE)
	echo '# Automatically-generated dependencies list:' > $(DEPENDSFILE)
	$(CXX) $(CCFLAGS) $(INCLUDE) -M $(LIBSRC) >> $(DEPENDSFILE)

$(LIBDIR)/libqUtilities.a: $(LIBOBJ) $(MOCOBJ)
	$(AR) $(ARFLAGS) $@ $(LIBOBJ) $(MOCOBJ)

include $(DEPENDSFILE)

mark:
	@echo "[0;34m============================================= [0m"


#-----------------------------------------------------------------------

mocs:	
	echo "#automatic generated moc targets " > $(MOCFILE)
	for ENTRY in $(LIBOBJ:.o=.h) ; \
	do \
	echo                                                        >> $(MOCFILE) ; \
	echo $${ENTRY%.h}_moc.cc: ../include/$${ENTRY}              >> $(MOCFILE) ; \
	echo "	"$(MOC) ../include/$${ENTRY} -o $${ENTRY%.h}_moc.cc >> $(MOCFILE) ; \
	done

-include $(MOCFILE)


#------------------------------------------------------------------------


LANGUAGES= $(LANGDIR)/qUtilities_no.ts

languages:
	$(QTDIR)/bin/lupdate  -verbose $(LIBSRC) -ts $(LANGUAGES)
	cd $(LANGDIR) ; 
	$(QTDIR)/bin/lrelease -verbose $(LANGUAGES)

