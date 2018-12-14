#include "AutorizationController.cpp"

int main(int argc, char const *argv[])
{
    std::string a = std::string("Ala");
    std::string haslo = std::string("MaKota");

    AutorizationController t = AutorizationController();
    t.isLoggedIn(a);
   /* t.logIn(a, haslo); */
    t.logOut(a);
    return 0;
}
