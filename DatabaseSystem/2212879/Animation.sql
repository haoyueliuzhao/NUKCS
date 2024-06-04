DROP DATABASE IF EXISTS Animation;
CREATE DATABASE Animation;
USE Animation;

-- 创建 Studio 表，存储制作公司信息
DROP TABLE IF EXISTS `Studio`;
CREATE TABLE Studio (
    studio_id INTEGER PRIMARY KEY AUTO_INCREMENT, -- 主键，自增长
    name TEXT NOT NULL,                          -- 名称，不能为空
    studio_synopsis TEXT,                              -- 简介
    founded_date DATE,                           -- 成立日期
    country TEXT                                 -- 国家
);

-- 插入制作公司信息
INSERT INTO Studio (name, studio_synopsis,founded_date, country)
VALUES 
    ('Wit Studio', 'shgvdlgvlsdgvbsilvgfi','2012-06-01', 'Japan'),
    ('Studio Pierrot','skgvskvgdslvgsd,lgv ', '1979-09-10', 'Japan'),
    ('Toei Animation','skgvb,isgvdxsigv,,sfz', '1948-01-23', 'Japan');
-- 创建 Anime 表，存储动漫信息
DROP TABLE IF EXISTS `Anime`;
CREATE TABLE Anime (
    anime_id INTEGER PRIMARY KEY AUTO_INCREMENT, -- 主键，自增长
    title TEXT NOT NULL,                        -- 标题，不能为空
    synopsis TEXT,                              -- 简介
    genre TEXT,                                 -- 类型
    release_date DATE,                          -- 发布日期
    episodes INTEGER,                           -- 集数
    status TEXT NOT NULL CHECK (status IN ('Airing', 'Completed', 'Upcoming')), -- 状态，只能为 'Airing', 'Completed', 'Upcoming' 中的一个
    rating DECIMAL(4,2),                        -- 评分
    studio_id INTEGER,                          -- 制作公司ID，外键关联 Studio 表
    cover_image TEXT,                           -- 封面图片
    FOREIGN KEY (studio_id) REFERENCES Studio(studio_id)
);

-- 插入动漫信息
INSERT INTO Anime (title, synopsis, genre, release_date, episodes, status, rating, studio_id, cover_image) 
VALUES 
    ('Attack on Titan', 'Humans living behind enormous walls to protect themselves from Titans.', 'Action, Fantasy', '2013-04-07', 25, 'Completed', 8.9, 1, 'attack_on_titan.png'),
    ('Naruto', 'A young ninja seeks to gain recognition and become the strongest ninja in his village.', 'Action, Adventure', '2002-10-03', 220, 'Completed', 8.90, 2, 'naruto.png'),
    ('One Piece', 'A pirates adventure to find the legendary One Piece treasure.', 'Action, Adventure, Comedy', '1999-10-20', 990, 'Airing', 8.9, 3, 'one_piece.png');

-- 创建 VoiceActor 表，存储配音演员信息
DROP TABLE IF EXISTS `VoiceActor`;
CREATE TABLE VoiceActor (
    voice_actor_id INTEGER PRIMARY KEY AUTO_INCREMENT, -- 主键，自增长
    name TEXT NOT NULL,                              -- 名称，不能为空
    VoiceActor_synopsis TEXT ,
    birthdate DATE,                                  -- 出生日期
    nationality TEXT                                 -- 国籍
);

-- 插入配音演员信息
INSERT INTO VoiceActor (name,VoiceActor_synopsis, birthdate, nationality)
VALUES
('Yuki Kaji','sfvsvfsffsfvsfv' ,'1985-09-03', 'Japan'),
('Maile Flanagan', 'sfvsffsfbbsvsfv','1965-05-19', 'United States');

-- 创建 Character 表，存储角色信息
DROP TABLE IF EXISTS `Character`;
CREATE TABLE `Character` (
    character_id INTEGER PRIMARY KEY AUTO_INCREMENT, -- 主键，自增长
    name TEXT NOT NULL,                             -- 名称，不能为空
    role TEXT,                                      -- 角色类型
    Character_synopsis TEXT,                             -- 简介
    anime_id INTEGER,                               -- 所属动漫ID，外键关联 Anime 表
    voice_actor_id INTEGER,                         -- 配音演员ID，外键关联 VoiceActor 表,
    FOREIGN KEY (anime_id) REFERENCES Anime(anime_id) ON DELETE CASCADE, -- 动漫删除时，级联删除角色
    FOREIGN KEY (voice_actor_id) REFERENCES VoiceActor(voice_actor_id) ON DELETE CASCADE -- 配音演员删除时，级联删除角色
);

-- 插入角色信息
INSERT INTO `Character` (name, role,Character_synopsis,anime_id,voice_actor_id)
VALUES
('Eren Yeager', 'Main','nnnsgfskgfvsfdvsldgdcsl', 1, 1),
('Naruto Uzumaki', 'Main','nnndhbvdlhbtdtbbdbdbb', 2, 2),
('Monkey D. Luffy', 'Main','nnnfhgvssuhgvdlshvlsh', 3, 2);


-- 创建 User 表，存储用户信息
DROP TABLE IF EXISTS `User`;
CREATE TABLE User (
    user_id INTEGER PRIMARY KEY AUTO_INCREMENT,  -- 主键，自增长
    username VARCHAR(255) NOT NULL UNIQUE,       -- 用户名，不能为空且唯一，最大长度为255
    password TEXT NOT NULL,                      -- 密码，不能为空
    email VARCHAR(255) NOT NULL UNIQUE,          -- 电子邮件，不能为空且唯一，最大长度为255
    joined_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP -- 加入时间，默认为当前时间
);

-- 插入用户信息
INSERT INTO User (username, password, email)
VALUES
('animefan123', 'password123', 'animefan123@example.com'),
('otakuGirl', 'otaku123', 'otakuGirl@example.com');


-- 创建 Review 表，存储用户对动漫的评论和评分
DROP TABLE IF EXISTS `Review`;
CREATE TABLE Review (
    review_id INTEGER PRIMARY KEY AUTO_INCREMENT, -- 主键，自增长
    user_id INTEGER,                            -- 用户ID，外键关联 User 表
    anime_id INTEGER,                           -- 动漫ID，外键关联 Anime 表
    rating DECIMAL(4,2),                        -- 评分
    comment TEXT,                               -- 评论
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- 创建时间，默认为当前时间
    FOREIGN KEY (user_id) REFERENCES User(user_id) ON DELETE CASCADE, -- 用户删除时，级联删除评论
    FOREIGN KEY (anime_id) REFERENCES Anime(anime_id) ON DELETE CASCADE -- 动漫删除时，级联删除评论
);


-- 插入评论信息
INSERT INTO Review (user_id, anime_id, rating, comment)
VALUES
(1, 1, 8.0, 'Absolutely loved the storyline and character development!'),
(2, 2, 8.0, 'Naruto is a classic, but sometimes the filler episodes are too much.');





-- 创建 Tag 表，存储标签信息
DROP TABLE IF EXISTS `Tag`;
CREATE TABLE Tag (
    tag_id INTEGER PRIMARY KEY AUTO_INCREMENT, -- 主键，自增长
    name VARCHAR(255) NOT NULL UNIQUE           -- 名称，不能为空且唯一，最大长度为255
);

-- 插入标签信息
INSERT INTO Tag (name)
VALUES
('Shounen'),
('Action'),
('Adventure');


-- 创建 AnimeTag 表，存储动漫和标签的关联关系
DROP TABLE IF EXISTS `AnimeTag`;
CREATE TABLE AnimeTag (
    anime_id INTEGER,                 -- 动漫ID
    tag_id INTEGER,                   -- 标签ID
    PRIMARY KEY (anime_id, tag_id),   -- 主键由 anime_id 和 tag_id 组成
    FOREIGN KEY (anime_id) REFERENCES Anime(anime_id) ON DELETE CASCADE, -- 动漫删除时，级联删除关联关系
    FOREIGN KEY (tag_id) REFERENCES Tag(tag_id) ON DELETE CASCADE -- 标签删除时，级联删除关联关系
);

-- 插入动漫和标签的关联关系
INSERT INTO AnimeTag (anime_id, tag_id)
VALUES
(1, 1),
(1, 2),
(2, 1),
(2, 2),
(2, 3),
(3, 1),
(3, 2),
(3, 3);

-- 创建 Favorite 表，存储用户收藏的动漫信息
DROP TABLE IF EXISTS `Favorite`;
CREATE TABLE Favorite (
    user_id INTEGER,                            -- 用户ID，外键关联 User 表
    anime_id INTEGER,                           -- 动漫ID，外键关联 Anime 表
    added_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- 添加时间，默认为当前时间
    PRIMARY KEY (user_id, anime_id),
    FOREIGN KEY (user_id) REFERENCES User(user_id) ON DELETE CASCADE, -- 用户删除时，级联删除收藏
    FOREIGN KEY (anime_id) REFERENCES Anime(anime_id) ON DELETE CASCADE -- 动漫删除时，级联删除收藏
);

-- 插入用户收藏信息
INSERT INTO Favorite (user_id, anime_id)
VALUES
(1, 1),
(2, 3);




-- 创建 UserLog 表，存储用户注册日志
DROP TABLE IF EXISTS `UserLog`;
CREATE TABLE UserLog (
    log_id INTEGER PRIMARY KEY AUTO_INCREMENT,  -- 主键，自增长
    user_id INTEGER,                            -- 用户ID，外键关联 User 表
    action TEXT NOT NULL,                       -- 用户操作类型
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- 操作时间，默认为当前时间
    FOREIGN KEY (user_id) REFERENCES User(user_id) ON DELETE CASCADE -- 用户删除时，级联删除日志
);

-- 用户登录/注册/登出记录触发器
DELIMITER $$
CREATE PROCEDURE log_user_action(IN p_user_id INTEGER, IN p_action TEXT)
BEGIN
    INSERT INTO UserLog (user_id, action, timestamp) 
    VALUES (p_user_id, p_action, CURRENT_TIMESTAMP);
END$$
DELIMITER ;


-- 更新动漫评论的触发器
DELIMITER $$
CREATE TRIGGER add_review_trigger
AFTER INSERT ON Review
FOR EACH ROW
BEGIN
    -- 检查评分是否在正常范围内
    IF NEW.rating < 0 OR NEW.rating > 10 THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Rating should be between 0 and 10';
    END IF;

END$$
DELIMITER ;

-- 添加评论时更新动漫评分的触发器
DELIMITER $$
CREATE TRIGGER add_review_update_rating
AFTER INSERT ON Review
FOR EACH ROW
BEGIN
    DECLARE avg_rating DECIMAL(4,2);
    -- 计算新的平均评分
    SELECT AVG(rating) INTO avg_rating FROM Review WHERE anime_id = NEW.anime_id;
    -- 更新动漫评分
    UPDATE Anime SET rating = avg_rating WHERE anime_id = NEW.anime_id;
END$$
DELIMITER ;


-- 删除评论时更新动漫评分的触发器
DELIMITER $$
CREATE TRIGGER delete_review_update_rating
AFTER DELETE ON Review
FOR EACH ROW
BEGIN
    DECLARE avg_rating DECIMAL(4,2);
    SELECT AVG(rating) INTO avg_rating FROM Review WHERE anime_id = OLD.anime_id;
    UPDATE Anime SET rating = avg_rating WHERE anime_id = OLD.anime_id;
END$$
DELIMITER ;



DELIMITER $$
CREATE PROCEDURE update_anime_rating(IN p_anime_id INT)
BEGIN
    DECLARE avg_rating DECIMAL(4,2);
    -- 检查动漫ID是否存在
    IF NOT EXISTS (SELECT * FROM Anime WHERE anime_id = p_anime_id) THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Anime ID does not exist';
    ELSE
        -- 计算新的平均评分
        SELECT AVG(rating) INTO avg_rating FROM Review WHERE anime_id = p_anime_id;
        -- 更新动漫评分
        UPDATE Anime SET rating = avg_rating WHERE anime_id = p_anime_id;
    END IF;
END$$
DELIMITER ;







