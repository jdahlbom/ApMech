GENERATED_HEADER=gen_ogreskeleton.h
GENERATED_NAME=gen_ogreskel
GSOAP_PATH=/usr/include/gsoap
SOAPCPP2_FLAGS=-C -w -p ${GENERATED_NAME} -I ${GSOAP_PATH}
SOURCE_XSD=../xsd/ogreskeletonxml.xsd
XSD_NAMESPACE=urn:apmech/skeletons
XSD_NS_PREFIX=genskel

TYPEMAP=typemap.dat

generate: clean generate_typemap generate_header generate_classes

clean:
	- rm ${TYPEMAP}
	- rm ${GENERATED_HEADER}
	- rm ${GENERATED_NAME}*
	- rm ${XSD_NS_PREFIX}.nsmap

generate_typemap:
	echo '${XSD_NS_PREFIX} = "${XSD_NAMESPACE}"' > ${TYPEMAP}

generate_header:
	wsdl2h ${SOURCE_XSD} -o ${GENERATED_HEADER}

generate_classes:
	soapcpp2 ${SOAPCPP2_FLAGS} ${GENERATED_HEADER}
	rm ${GENERATED_NAME}Client*
