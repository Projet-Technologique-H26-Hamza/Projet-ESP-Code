#ifndef LOGIN_H
#define LOGIN_H

#include <Arduino.h>

class Login {
public:
    Login();

    /**
     * Effectue le login auprès de l'API.
     * @param tokenOut Référence vers une String qui recevra le Token JWT.
     * @return 1 en cas de succès, -1 en cas d'erreur.
     */
    int LoginDispenser(String &tokenOut); 
};

#endif