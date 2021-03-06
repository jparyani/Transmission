/*
 * This file Copyright (C) 2009-2014 Mnemosyne LLC
 *
 * It may be used under the GNU GPL versions 2 or 3
 * or any future license endorsed by Mnemosyne LLC.
 *
 * $Id$
 */

#include "prefs.h"
#include "session.h"
#include "session-dialog.h"

/***
****
***/

void
SessionDialog::onAccepted ()
{
  myPrefs.set (Prefs::SESSION_IS_REMOTE, ui.remoteSessionRadio->isChecked ());
  myPrefs.set (Prefs::SESSION_REMOTE_HOST, ui.hostEdit->text ());
  myPrefs.set (Prefs::SESSION_REMOTE_PORT, ui.portSpin->value ());
  myPrefs.set (Prefs::SESSION_REMOTE_AUTH, ui.authCheck->isChecked ());
  myPrefs.set (Prefs::SESSION_REMOTE_USERNAME, ui.usernameEdit->text ());
  myPrefs.set (Prefs::SESSION_REMOTE_PASSWORD, ui.passwordEdit->text ());
  mySession.restart ();
  hide ();
}

void
SessionDialog::resensitize ()
{
  const bool isRemote = ui.remoteSessionRadio->isChecked ();
  const bool useAuth = ui.authCheck->isChecked ();

  foreach (QWidget * w, myRemoteWidgets)
    w->setEnabled (isRemote);

  foreach (QWidget * w, myAuthWidgets)
    w->setEnabled (isRemote && useAuth);
}

/***
****
***/

SessionDialog::SessionDialog (Session& session, Prefs& prefs, QWidget * parent):
  QDialog (parent),
  mySession (session),
  myPrefs (prefs)
{
  ui.setupUi (this);

  ui.localSessionRadio->setChecked (!prefs.get<bool> (Prefs::SESSION_IS_REMOTE));
  connect (ui.localSessionRadio, SIGNAL (toggled (bool)), this, SLOT (resensitize ()));

  ui.remoteSessionRadio->setChecked (prefs.get<bool> (Prefs::SESSION_IS_REMOTE));
  connect (ui.remoteSessionRadio, SIGNAL (toggled (bool)), this, SLOT (resensitize ()));

  ui.hostEdit->setText (prefs.get<QString> (Prefs::SESSION_REMOTE_HOST));
  myRemoteWidgets << ui.hostLabel << ui.hostEdit;

  ui.portSpin->setValue (prefs.get<int> (Prefs::SESSION_REMOTE_PORT));
  myRemoteWidgets << ui.portLabel << ui.portSpin;

  ui.authCheck->setChecked (prefs.get<bool> (Prefs::SESSION_REMOTE_AUTH));
  connect (ui.authCheck, SIGNAL (toggled (bool)), this, SLOT (resensitize ()));
  myRemoteWidgets << ui.authCheck;

  ui.usernameEdit->setText (prefs.get<QString> (Prefs::SESSION_REMOTE_USERNAME));
  myAuthWidgets << ui.usernameLabel << ui.usernameEdit;

  ui.passwordEdit->setText (prefs.get<QString> (Prefs::SESSION_REMOTE_PASSWORD));
  myAuthWidgets << ui.passwordLabel << ui.passwordEdit;

  resensitize ();

  connect (ui.dialogButtons, SIGNAL (rejected ()), this, SLOT (hide ()));
  connect (ui.dialogButtons, SIGNAL (accepted ()), this, SLOT (onAccepted ()));
}
