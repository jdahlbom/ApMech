GENERATED_HEADER=gen_mech.h
GENERATED_NAME=gen_mech
GSOAP_PATH=/usr/include/gsoap
SOAPCPP2_FLAGS=-C -w -I ${GSOAP_PATH} -p ${GENERATED_NAME}
SOURCE_XSD=../xsd/mech.xsd
XSD_NAMESPACE=urn:apmech/mechs
XSD_NS_PREFIX=genmech

TYPEMAP=typemap.dat

generate: clean generate_typemap generate_header generate_classes

clean:
	- rm ${TYPEMAP}
	- rm ${GENERATED_HEADER}
	- rm ${GENERATED_NAME}*
	- rm ${XSD_NS_PREFIX}.nsmap

generate_typemap:
	echo '${XSD_NS_PREFIX} = "${XSD_NAMESPACE}"' > ${TYPEMAP}

generate_header: generate_typemap
	wsdl2h ${SOURCE_XSD} -o ${GENERATED_HEADER} 

generate_classes: generate_header
	soapcpp2 ${SOAPCPP2_FLAGS} ${GENERATED_HEADER}
	rm ${GENERATED_NAME}Client*
