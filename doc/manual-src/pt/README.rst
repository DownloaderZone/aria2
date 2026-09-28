.. _README:

.. meta::
   :description lang=pt: Manual Aria2 em português
   :keywords: programa para download gratuito,
              linha de comando, download de músicas, download de ftp,
              download http, download https, mac OS/X, windows, linux,
              manual download aria2, download stream
   :author: tatsuhiro.t_at_gmail_dot_com english version
   :author: gsavix@gmail.com tradução para português do brasil


aria2 - Utilitário para Download Super Ultra Rápido
===================================================
:Author:     Tatsuhiro Tsujikawa
:Email:      tatsuhiro.t_at_gmail_dot_com
:translator: pt_BR Portuguese, tradutor: gsavix@gmail.com

.. index:: double: author; tatsuhiro.t_at_gmail_dot_com
.. index:: triple: tradutor; tradução; gsavix@gmail.com;

Renúncia
--------
Este programa não vem com garantias. O uso deste programa é por sua
conta e risco.

Introdução
----------
aria2 é um utilitário para download de arquivos. Os protocolos suportados são
HTTP, HTTPS, FTP, SFTP e Metalink. aria2 pode baixar arquivos de
múltiplas fontes protocolos e tenta utilizar para isso a máxima banda possível.
Pode funcionar em diversas plataformas de computadores e sistemas operacionais,
como por exemplo: GNU Linux, OS X, Windows entre outros. Ao mesmo
tempo pode executar download de HTTP, HTTPS, FTP e SFTP, e você pode
ir assistindo um filme enquanto o download prossegue.  Através da
verificação (checksum) de partes dos dados dos Metalink's, aria2
automaticamente valida as partes dos dados durante o download.

A página do projeto está em
https://github.com/DownloaderZone/aria2.

As fontes do manual (inglês, russo e português) estão em
``doc/manual-src``.  Execute ``make html`` para gerar o manual HTML;
consulte a seção `Para Gerar a documentação`_.

Funcionalidades
---------------

Lista de Configurações:

* Interface de linha de comando
* Download arquivos protocolos HTTP, HTTPS, FTP, SFTP
* Download Segmentado
* Metalink versão 4 (RFC 5854) suporte (HTTP, FTP, SFTP)
* Metalink versão 3 suporte (HTTP, FTP, SFTP)
* Metalink (RFC 6249) suporte (HTTP)
* Implementação HTTP/1.1
* Suporte Proxy HTTP Proxy
* Suporte autenticação HTTP BASIC
* Suporte autenticação HTTP Proxy
* Váriavéis de ambiente (conjunto abrangente) para proxy: http_proxy, https_proxy,
  ftp_proxy, all_proxy e no_proxy
 
* HTTP com gzip, suporte a codificação de conteúdo (deflate)
* Verificação de peer usando Certificados Acreditados informado em HTTPS
* Processamento de autenticação Cliente usando Certificado Acreditado HTTPS
* Suporte a transferência (Chunked) codificada
* Carga de Cookies a partir de arquivos formato Firefox 3, Chromium/Google Chrome
  e Mozilla / Firefox / IcedWeasel / (1.x/2.x) / Netscape.
  
* Salvar Cookies em arquivo formato Mozilla 3, Firefox (1.x/2.x), Chromium,
  Netscape.
 
* Suporte a Cabeçalho HTTP modificado ou personalizado
* Suporte a Conexões Persistentes
* FTP através de Proxy HTTP
* Controle de velocidade de Download (utilização da Banda Rede)
* Interface JSON-RPC (sobre HTTP e WebSocket) / XML-RPC
* Execução em modo daemon
* Download Seletivo em múltiplos arquivos Metalink
* Validação e checksum de parte (Chunk) em Metalink
* Desabilidar download segmentado em Metalink
* Suporte a Netrc
* Suporte a arquivo de Configuração
* Download de URIs a partir de arquivo texto ou entrada padrão com especificação
  opcional de arquivo de saída
 
* Suporte a URI parametrizadas (intervalos, etc)
* Suporte a IPv6
 
Como obter o código fonte
-------------------------

O código fonte é mantido no Github:
https://github.com/DownloaderZone/aria2

Para obter o último código fonte, execute o seguinte comando::

    $ git clone https://github.com/DownloaderZone/aria2.git

Aria2 será criado no diretório corrente do seu computador com os arquivos fonte.


Dependências
------------


======================== ========================================
funcionalidade           dependência
======================== ========================================
HTTPS                    GnuTLS ou OpenSSL
Metalink                 libxml2 ou Expat.
Checksum                 libnettle ou libgcrypt ou OpenSSL
gzip, deflate em HTTP    zlib
Async DNS                C-Ares
Firefox3/Chromium cookie libsqlite3
XML-RPC                  libxml2 ou Expat.
JSON-RPC sobre WebSocket libnettle ou libgcrypt ou OpenSSL
======================== ========================================


.. note::

  libxml2 tem precedência sobre Expat se ambas bibliotecas instaladas.
  Caso prefira Expat, execute o configure com ``--without-libxml2``.

.. note::

  GnuTLS tem precedência sobre OpenSSL se ambas bibliotecas estiverem
  instaladas. Caso prefira OpenSSL, execute configure com 
  ``--without-gnutls`` ``--with-openssl``.

.. note::

  libnettle tem precedência sobre libgcrypt se ambas bibliotecas instaladas.
  Caso prefira libgcrypt, execute configure com 
  ``--without-libnettle --with-libgcrypt``. Se OpenSSL é selecionada em
  detrimento de GnuTLS, nem libnettle nem libgcrypt poderão ser usadas.

São necessárias as seguintes configurações de bibliotecas SSl e crypto:

* libgcrypt
* libnettle
* OpenSSL
* GnuTLS + libgcrypt
* GnuTLS + libnettle

Metalink pode ser desabilitado fornecendo-se a opção
``--disable-metalink`` ao executar o script 
./configure.


Para habilitar DNS assíncrono é necessário c-ares.

* projeto c-ares: http://daniel.haxx.se/projects/c-ares/

Como fazer o build
------------------
Para fazer o build do aria2 a partir dos fontes, instalar antes
pacotes de desenvolvimento ( o nome pode variar nas distribuições):

* libgnutls-dev    (Requerido para suporte HTTPS, Checksum)
* nettle-dev       (Requerido para suporte Checksum)
* libc-ares-dev    (Requerido para suporte DNS async DNS )
* libxml2-dev      (Requerido para suporte Metalink)
* zlib1g-dev       (Requerido para suporte em HTTP decodificação gzip e deflate)
* libsqlite3-dev   (Requerido para suporte cookie Firefox3 / Chromium)

Você pode usar libgcrypt-dev ao invés de nettle-dev:

* libgpg-error-dev (Requerido para suporte Checksum)
* libgcrypt-dev    (Requerido para suporte Checksum)

Pode ser usado libssl-dev ao invés de
libgnutls-dev, nettle-dev, libgpg-error-dev e libgcrypt-dev:

* libssl-dev       (Requerido para suporte HTTPS, Checksum)

Pode ser usado libexpat1-dev ao invés de libxml2-dev:

* libexpat1-dev    (Requerido for Metalink suporte)

Pode ser necessário usar pkg-config para detectar as bibliotecas
acima mencionadas.

Para a distribuição Fedora são necessários os seguintes pacotes:

* gcc, gcc-c++, kernel-devel, libgcrypt-devel, libgcrypt-devel, 
  libxml2-devel, openssl-devel

Se foi feito o download do código fonte a partir de um repositório git,
deverá ser executado o seguinte comando para criar o script de
configuração (configure) e outros scripts também necessários
para o build ou compilação do aria2::

    $ autoreconf -i

Para construir a documentação é necessário instalar
`Sphinx <http://sphinx.pocoo.org/>`_ que constroi página (man), html ou pdf
contendo a documentação nos idiomas existentes.

Para construir aria2 para Mac OS X, de uma olhada em build_osx_release.sh,
o qual cria binários DMG que são padrões para OS X.

A maneira mais rápida para compilar o aria2 é executar o script::

    $ ./configure

Para compilar com link-edição estática utilize o opção da linha
de comando ``ARIA2_STATIC=yes``::

    $ ./configure ARIA2_STATIC=yes

Após a configuração feita, execute ``make`` para compilar o programa::

    $ make

Ver `Compilação Cross Windows binário`_ para Criar Binário para
Windows.
O script configure verifica as bibliotecas
disponíveis e habilita
ou desabilita as funcionalidades na maior abrangência possivel, pois
por padrão todas as funcionalidades são habilitadas.  Desde a versão
1.1.0, aria2 verifica o certificado do servidor HTTPS como padrão.
Se a compilação usou OpenSSL ou a recente versão de GnuTLS na qual há
função ``gnutls_certificate_set_x509_system_trust()`` e a biblioteca
foi adequadamente configurada para localizar o certificado CA
armazenado, aria2 carrega automaticamente estes certificados no início.
Se este não ocorrer, recomenda-se fornecer o caminho para o arquivo que
contém o certificado. Por Exemplo, no Debian o caminho para o arquivo CA
é "/etc/ssl/certs/ca-certificates.crt" (no pacote ca-certificates).
Isto varia de acordo com sua distribuição Linux. Pode ser informada a opção
``--with-ca-bundle`` para configurar o script usado pelo make:: 

    $ ./configure --with-ca-bundle='/etc/ssl/certs/ca-certificates.crt'
    $ make

Sem a opção ``--with-ca-bundle``, haverá um erro ao acessar servidores
HTTPS pois o certificado não pode ser verificado sem possuir um pacote CA
(entidade certificadora). Nesse caso, pode ser especificado um arquivo de
certificado usando opção aria2's ``--ca-certificate``.  Caso não haja um
certificado instalado, então o último recurso é desabilitar a validação
do certificado usando a opção ``--check-certificate=false``.

Por padrão, arquivo chamado (bash_completion) ``aria2c`` é instalado no
diretório ``$prefix/share/doc/aria2/bash_completion``.  Para mudar o
diretório de instalação do arquivo utilize a opção
``--with-bashcompletiondir``.

O executavel é 'aria2c' no diretório src.

aria2 usa CppUnit para (test unit) automatizado. Para executar o
test unit emita o comando::

    $ make check

Compilação Cross Windows binário
--------------------------------

Nessa seção, está descrito como fazer o build do binário para Windows
usando o compilador cross mingw-w64 no Debian Linux.

Basicamente, após compilar e instalar as bibliotecas dependentes, que
são pré-requisitos pode ser feita a compilação cross apenas passando 
através da opção ``--host`` e especificando-se as variáveis
``CPPFLAGS``, ``LDFLAGS`` e ``PKG_CONFIG_LIBDIR`` que serão usadas
no procedimento (configure). Para maior conveniência e menor custo
de desenvolvimento, é fornecida uma maneira fácil de configurar as
características do build / compilação.

O script ``mingw-config`` é um ``(wrapper)`` para mingw-w64.
Sua utilização é para gerar uma compilação oficial para Windows.  Esse
script assume que as seguintes bibliotecas tenham sido compiladas
e/ou instaladas para a compilação cross:

* c-ares
* openssl
* expat
* sqlite3
* zlib
* cppunit

Algumas variáveis de ambiente precisam ser ajustadas para compilar:

``HOST``
  compilação-cross para compilar programas que serão executados em
  um computador ``HOST``. Padrão para ``i686-w64-mingw32``.
  Para compilar binário para 64bits, especificar ``x86_64-w64-mingw32``.

``PREFIX``
  Prefixo do diretório onde as bibliotecas dependentes estão instaladas.
  Padrão para ``/usr/local/$HOST``. ``-I$PREFIX/include`` será adicionado
  às opções ``CPPFLAGS``. ``-L$PREFIX/lib`` será adicionado em 
  ``LDFLAGS``. ``$PREFIX/lib/pkgconfig`` será configurado para 
  ``PKG_CONFIG_LIBDIR``.

Por exemplo, para construir um binário para 64bits utilize:: 

    $ HOST=x86_64-w64-mingw32 ./mingw-config

Para Gerar a documentação
=========================

`Sphinx <http://sphinx.pocoo.org/>`_ é usado para construir a
documentação. As páginas (man) da documentação também são criadas
através do comando ``make man`` caso estas páginas estejam desatualizadas.
Também podem ser construídas a documentação em diversos formatos como
html, latexpdf, epub, devhelp, latex, htmlhelp. Exemplo: Para construir
a documentação aria2 em HTML use o comando ``make html``.
O resultado será criado na subpasta _build dentro da pasta onde baixou
o fonte do aria2. Ex: se baixou e descompactou o aria2 no local
/home/usuario/aria2/ então a documentação gerada está em::

/home/usuario/aria2/doc/manual-src/pt/_build/

A pasta ``pt`` indica o idioma que atualmente pode ser
(en,pt,ru). Para gerar documento PDF use a opção ``make latexpdf``
certifique-se de que os pré-requisitos de fontes usados pelo latex 
estejam presentes.

Não há uma versão online hospedada deste manual; gere-o localmente
com ``make html`` como descrito acima.

Metalink
========

A implementação atual suporte HTTP, HTTPS, FTP e SFTP.  Outros
protocolos P2P são ignorados. São suportados documentos das versões
Metalink4 e Metalink 3.0.

Para validação de (checksum) são suportados: md5, sha-1, sha-224,
sha-256, sha-384 e sha-512. Se múltiplos algoritmos de hast
são fornecidos aria2 utiliza o mais robusto. Se a validação do
checksum falhar totalmente aria2 não tentará mais fazer download e
terminará o processamento com código de retorno diferente de zero.

As preferências de usuário suportadas são versão, idioma, local,
protocolo e sistema operacional.

Se verificação (checksum) de (chunk) são fornecidas em um arquivo
Metalink, aria2 automaticamente valida (chunk) partes dos dados
durante o download. Esse comportamente pode ser desligado através
de opção da linha de comando.

Se uma assinatura (certificado) é incluida em um arquivo Metalink,
aria2 salva a assinatura como um arquivo após a conclusão do download.
O nome do arquivo terá o sufixo ".sig". Caso já exista não será salvo.

Em Metalink4, um elemento metalink:metaurl pode apontar para um tipo de
recurso que aria2 não suporta, tais como um download BitTorrent.  Esses
recursos são ignorados, e aria2 baixa o arquivo utilizando as outras URIs
encontradas no documento.

Se uma URI relativa é especificada em um elemento metalink:url ou
metalink:metaurl, aria2 usa a URI do arquivo Metalink como URI base
para resolver a URI relativa. Se a URI relativa encontra-se em um 
arquivo Metalink que é lido do disco local, aria2 usa o valor da
opção ``--metalink-base-uri`` como URI base.  Se essa opção não é
especificada a URI relativa será ignorada.

Metalink / HTTP
===============

Esta versão utiliza links rel=duplicate. aria2 interpreta
os campos do cabeçalho do Digest e verifica onde o valor do digest
confere com outras fontes. Se houver diferença, derruba a conexão.
aria2 também utiliza esse valor do digest para executar verificação do
checksum após o download terminar. aria2 reconhece valor geo.
Para sobrepor o valor de sua preferência utilize a opção
``--metalink-location``.

netrc
=====
O suporte netrc é habilitado por padrão para HTTP, HTTPS e FTP.  Para 
desabilitar especificar opção -n na linha de comando. 
Seu arquivo .netrc precisa possuir permissões corretas (600).

WebSocket
=========

O servidor WebSocket intrínseco no aria2 implementa a especificação
definida na RFC 6455. O protocolo suportado refere-se a versão 13.

Referências
===========

* `Código fonte do aria2 
  <https://github.com/DownloaderZone/aria2>`_ 
  
* `Releases do aria2 
  <https://github.com/DownloaderZone/aria2/releases>`_ 
  
* `Fontes do manual (en, pt, ru) 
  <https://github.com/DownloaderZone/aria2/tree/master/doc/manual-src>`_
* `RFC 959 FILE TRANSFER PROTOCOL (FTP) 
  <http://tools.ietf.org/html/rfc959>`_
* `RFC 1738 Uniform Resource Locators (URL) 
  <http://tools.ietf.org/html/rfc1738>`_
* `RFC 2428 FTP Extensions for IPv6 and NATs 
  <http://tools.ietf.org/html/rfc2428>`_
* `RFC 2616 Hypertext Transfer Protocol -- HTTP/1.1 
  <http://tools.ietf.org/html/rfc2616>`_
* `RFC 3659 Extensions to FTP <http://tools.ietf.org/html/rfc3659>`_
* `RFC 3986 Uniform Resource Identifier (URI): Generic Syntax 
  <http://tools.ietf.org/html/rfc3986>`_
* `RFC 4038 Application Aspects of IPv6 Transition
  <http://tools.ietf.org/html/rfc4038>`_
* `RFC 5854 The Metalink Download Description Format 
  <http://tools.ietf.org/html/rfc5854>`_
* `RFC 6249 Metalink/HTTP: Mirrors and Hashes 
  <http://tools.ietf.org/html/rfc6249>`_
* `RFC 6265 HTTP State Management Mechanism 
  <http://tools.ietf.org/html/rfc6265>`_
* `RFC 6455 The WebSocket Protocol <http://tools.ietf.org/html/rfc6455>`_

versão revisada  em 30.março.2013    por gsavix@gmail.com

Anotação sobre divergência entre Manual e o aria2:

Esta página de manual pode não necessariamente conter a última informação.
Caso haja discrepância entre alguma informação do manual e o aria2, por
favor refira-se a versão em inglês resultante do comando ``man aria2c``

