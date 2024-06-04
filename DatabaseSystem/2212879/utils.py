import hashlib  # 导入哈希库

def hash_password(password):
    return hashlib.sha256(password.encode()).hexdigest()  # 使用SHA-256算法哈希密码

def check_password(hashed_password, user_password):
    return hashed_password == hashlib.sha256(user_password.encode()).hexdigest()  # 校验密码是否匹配
