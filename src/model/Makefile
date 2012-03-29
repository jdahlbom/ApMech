GENERATED_HEADER=datamodel_h.h
GENERATED_NAME=datamodel
GSOAP_PATH=/usr/include/gsoap
SOAPCPP2_FLAGS=-C -w -I ${GSOAP_PATH} -p ${GENERATED_NAME}

TYPEMAP=typemap.dat

generate: clean generate_typemap generate_header generate_classes

clean:
	- rm ${TYPEMAP}
	- rm ${GENERATED_HEADER}
	- rm ${GENERATED_NAME}*
	- rm ${XSD_NS_PREFIX}.nsmap

generate_typemap:
	- rm ${TYPEMAP}
	echo 'genmech = "urn:apmech/mechs/1"' >> ${TYPEMAP}

generate_header: generate_typemap
	wsdl2h xsd/mech.xsd -o ${GENERATED_HEADER} 

generate_classes: generate_header
	soapcpp2 ${SOAPCPP2_FLAGS} ${GENERATED_HEADER}
	rm ${GENERATED_NAME}Client*
