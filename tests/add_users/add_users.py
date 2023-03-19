import os
import requests
import json

'''
{
  "first_name": "Ivan",
  "second_name": "Ivanov",
  "age": 18,
  "male": true,
  "interests": "Хобби, интересы и т.п.",
  "city": "Москва",
  "password": "password",
  "email": "ivanov_eng@gmail.com"
}
'''

URL: str = 'http://127.0.0.1:80/user/register'
users_counter: int = 0


class User:
    __first_name: str = ''
    __second_name: str = ''
    __age: int = 0
    __male: bool = True
    __interests: str = 'интересы'
    __city: str = ''
    __password: str = '123456'
    __email: str = ''

    def __init__(self, first_name: str, second_name: str, age: int, city: str) -> None:
        global users_counter

        self.__first_name = first_name
        self.__second_name = second_name
        self.__age = age
        self.__city = city
        self.__email = f'user{users_counter}@mail.ru'

        users_counter += 1

    def to_json(self) -> dict[str, any]:
        return {
            "first_name": self.__first_name,
            "second_name": self.__second_name,
            "age": self.__age,
            "male": self.__male,
            "interests": self.__interests,
            "city": self.__city,
            "password": self.__password,
            "email": self.__email
        }


def make_user(text: str) -> User:
    text = text.replace('\n', '')
    text = text.split(',')
    name: list[str] = text[0].split(' ')

    return User(name[1], name[0], int(text[1]), text[2])


def main() -> None:
    try:
        counter: int = 0
        script_dir: str = os.path.dirname(os.path.abspath(__file__))

        with open(f'{script_dir}/people.csv', mode='r', encoding='utf-8') as file:
            while (line := file.readline()):
                user: User = make_user(line)
                response = requests.post(url=URL, json=user.to_json())
                print(f'Send {counter} user with respond: \'{response.text}\'')
                counter += 1
    except:
        print('Exception thrown. Finishing...')
    finally:
        print('Done')


if '__main__' == __name__:
    main()
