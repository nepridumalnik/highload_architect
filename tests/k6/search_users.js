import { check } from 'k6'
import http from 'k6/http'

export default function () {
    const url = 'http://localhost:80/user/search'
    const payload = {
        first_name: 'Иван%',
        second_name: 'Бород%'
    };

    const data = JSON.stringify(payload)

    const params = {
        headers: {
            'Content-Type': 'application/json',
        },
    };

    const res = http.post(url, data, params)

    check(res, {
        'status 200': (r) => r.status === 200,
    });
}
