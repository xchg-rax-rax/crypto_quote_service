#!/bin/bash

# Set the desired common name for the certificate
COMMON_NAME="localhost"

# Generate a private key
openssl genrsa -out private.key 2048

# Generate a certificate signing request (CSR)
openssl req -new -sha256 -key private.key -out csr.csr -subj "/CN=${COMMON_NAME}"

# Generate a self-signed certificate using the CSR and private key
openssl x509 -req -sha256 -days 365 -in csr.csr -signkey private.key -out certificate.crt

# Cleanup intermediate files
rm csr.csr

echo "Self-signed certificate and private key generated successfully."

