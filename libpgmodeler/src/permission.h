/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description:  Implements the operations to manipulate object permissions.
# Creation date: 16/09/2010
#
# Copyright 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/
#ifndef PERMISSION_H
#define PERMISSION_H

#include "baseobject.h"
#include "function.h"
#include "role.h"
#include <algorithm>
 #include <QTextStream>

class Permission: public BaseObject {
 /* Permissões no PostgreSQL se aplicam somente aos objetos
    de banco de dados como a seguir:
    * tabela
    * coluna
    * visão
    * sequencia
    * banco de dados
    * foreign-data wrapper (não implementado)
    * foreign server (não implementado)
    * large objects (não implementado)
    * função
    * função agregada
    * linguagem
    * esquema
    * espaço de tabela */
 private:
   //Objeto o qual se aplicam os privilégios
   BaseObject *objeto;

   /* Usuários/grupos os quais detém as permissões sobre o objeto.
      Este vetor pode estar vazio caso se deseja
      dar permissão a todos os usuários/grupos do cluster (PUBLIC)
      sobre o objeto */
   vector<Role *> papeis;

   //Conjunto de privilégios que se aplicam ao objeto
   bool privilegios[13];

   /* Indica se um privilégio no índice dado pode ser atribuído a outros papéis
      sobre o mesmo objeto papéis (WITH GRANT OPTION). Este atributo não se aplica
      quando não há um papel especificado (PUBLIC). Este atributo é ignorado
      quando não há papel definido como detentor dos privilégios. */
   bool op_concessao[13];

   /* Gera um identificador único para a permissão usando o atributo
      nome da classe base ObjetoBase este é usado apenas para evitar
      duplicidade de permissões no modelo */
   void gerarIdPermissao(void);

 public:
   //Constantes usadas para referencias cada tipo de privilégio
   static const unsigned PRIV_SELECT=0,
                         PRIV_INSERT=1,
                         PRIV_UPDATE=2,
                         PRIV_DELETE=3,
                         PRIV_TRUNCATE=4,
                         PRIV_REFERENCES=5,
                         PRIV_TRIGGER=6,
                         PRIV_CREATE=7,
                         PRIV_CONNECT=8,
                         PRIV_TEMPORARY=9,
                         PRIV_EXECUTE=10,
                         PRIV_USAGE=11;

  /* No construtor é obrigatório especificar qual objeto receberá as permissões
     não sendo possível alterar este objeto após a instância da classe
     ser criada. O que é possível é apenas manipular os papéis e privilégios
     relacionados ao objeto */
  Permission(BaseObject *objeto);

  //Define o papel que deterá os privilégios sobre o objeto
  void adicionarPapel(Role *papel);

  //Define um dado privilégio para o papel sobre o objeto
  void definirPrivilegio(unsigned privilegio, bool valor, bool op_concessao);

  //Remove um papel através de seu índice
  void removerPapel(unsigned idx_papel);

  //Remove todos os papeis da permissão
  void removerPapeis(void);

  //Obtém o número de papés detentores de privilégios sobre o objeto
  unsigned obterNumPapeis(void);

  //Obtém um papel que detém os privilégios sobre o objeto
  Role *obterPapel(unsigned idx_papel);

  //Obtém o objeto que está sujeito aos privilégios
  BaseObject *obterObjeto(void);

  //Obtém o estado atual do flag de opção de concessão de privilégios
  bool obterOpConcessao(unsigned privilegio);

  /* Obtém a situação do privilégio especificado, se o mesmo está
     ativo ou não para o papel */
  bool obterPrivilegio(unsigned privilegio);

  /* Retorna uma string contendo todos os privilégios
     configurados no formato interno de permissões do
     PostgreSQL (conforme documentação do comando GRANT) */
  QString obterStringPrivilegios(void);

  //Indica se o papel está referenciado na permissão
  bool papelReferenciado(Role *papel);

  //Retorna a definição SQL ou XML do objeto
  QString getCodeDefinition(unsigned tipo_def);
};

#endif