<?php

use Nette\Security;

class Authenticator implements \Nette\Security\IAuthenticator {

    private $userService;

    public function __construct(\Models\User $userService) {

        $this->userService = $userService;
    }

    public function authenticate(array $credentials) {

        list($email, $password) = $credentials;
        $user = $this->userService->getByEmail($email)->fetch();

        if (!$user) {
            throw new Nette\Security\AuthenticationException('User not found.');
        }

        if (!Nette\Security\Passwords::verify($password, $user->password)) {
            throw new Nette\Security\AuthenticationException('Invalid password.');
        }

        return new Nette\Security\Identity($user->id, $user->role, ['email' => $user->email]);
    }
}